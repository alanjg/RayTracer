#include "pch.h"
#include "RayTracer.h"
#include "stb_image_write.h"

RayTracer::RayTracer()
{
	scene_ = new Scene();
}

RayTracer::~RayTracer()
{
	delete scene_;
}

Scene* RayTracer::GetScene()
{
	return scene_;
}

bool RayTracer::LoadScene(const std::string& filename)
{
	std::ifstream infile(filename.c_str());
	if (!infile)
	{
		return false;
	}

	infile >> *scene_;
	scene_->BalanceTree();
	return true;
}

struct ThreadData
{
	RayTracer* rt;
	double* pixels;
	std::vector<Tile>* tiles;
	unsigned int* currentTile;
	std::mutex* tileMutex;
};

void RayThread(ThreadData data)
{
	bool hasMoreData = true;
	while (hasMoreData)
	{
		Tile tile;
		data.tileMutex->lock();
		if (*data.currentTile < data.tiles->size())
		{
			int oldPercent = 100 * (*data.currentTile) / data.tiles->size();
			tile = (*data.tiles)[*data.currentTile];
			++(*data.currentTile);
			int newPercent = 100 * (*data.currentTile) / data.tiles->size();

			if (newPercent > oldPercent)
			{
				std::cout << newPercent << "% " << std::endl;
			}			
		}
		else
		{
			hasMoreData = false;
		}
		data.tileMutex->unlock();
		
		if (hasMoreData)
		{
			data.rt->RenderTile(data.pixels, tile);
		}		
	}
}

void RayTracer::RenderTile(double* pixels, const Tile& tile)
{
	int width = tile.width;
	int height = tile.height;
	int wBegin = tile.wBegin;
	int wEnd = tile.wEnd;
	int hBegin = tile.hBegin;
	int hEnd = tile.hEnd;
	Camera& camera = scene_->GetCamera();

	double aspectRatio = float(width) / float(height);
	double yScale = tan(camera.fov / 2.0 * 3.14159f / 180.0f);
	double xScale = yScale * aspectRatio;

	Vector3 up = camera.up;
	up.Normalize();

	Vector3 dir = camera.centerOfInterest - camera.position;
	dir.Normalize();

	Vector3 right = dir.Cross(camera.up);
	right.Normalize();

	camera.SetViewport(width, height);
	float maxIntensity = 1.0;

	int percent = 0;
	for (int y = hBegin; y < hEnd; y++)
	{
		for (int x = wBegin; x < wEnd; x++)
		{
			Color color(0, 0, 0);
			for (int i = 0; i < global_options.num_samples; i++)
			{
				Ray ray;
				if (global_options.num_samples == 1)
				{
					// Don't jitter if only taking one sample
					ray = camera.CreateRay(x, y);
				}
				else
				{
					ray = camera.CreateRandomRay(x, y);
				}
				Color sample = SampleScene(ray);
				if (!isnan(sample.r) && !isnan(sample.g) && !isnan(sample.b))
				{
					if (sample.r > 1 || sample.g > 1 || sample.b > 1)
					{
					//	std::cout << "clamped " << sample << std::endl;
					}
					
					sample.Clamp(0, 1);
					if (sample.r == 0 && sample.g == 0 && sample.b == 0)
					{
						//std::cout << "black";
					}
					color += sample;
				}
				else
				{
					std::cout << "bad" << std::endl;
					//throw std::exception();
				}
			}
			
			Color rayColor = color * double(1.0 / global_options.num_samples);
			if (rayColor.r > 1 || rayColor.g > 1 || rayColor.b > 1)
			{
				std::cout << "clamped " << rayColor << std::endl;
			}
			rayColor.Clamp(0, 1);
			

			//sqrt for gamma correction
			pixels[y * (width * 3) + x * 3 + 0] = sqrt(rayColor.r);
			pixels[y * (width * 3) + x * 3 + 1] = sqrt(rayColor.g);
			pixels[y * (width * 3) + x * 3 + 2] = sqrt(rayColor.b);
		}
	}
}

void RayTracer::RenderToPixels(double* pixels, int width, int height)
{
	memset(pixels, 0, width * height * 3 * sizeof(double));
	std::vector<Tile> tiles;
	for(int x = 0; x < width; x += 16)
	{
		for (int y = 0; y < height; y += 16)
		{
			Tile tile;
			tile.width = width;
			tile.wBegin = x;
			tile.wEnd = std::min(x + 16, width);
			tile.height = height;
			tile.hBegin = y;
			tile.hEnd = std::min(y + 16, height);
			tiles.push_back(tile);
		}
	}
	std::vector<std::thread> threads;
	std::mutex tileMutex;
	unsigned int currentTile = 0;
	for (int i = 0; i < global_options.num_threads; i++)
	{
		ThreadData data;
		data.rt = this;
		data.pixels = pixels;
		data.tiles = &tiles;
		data.tileMutex = &tileMutex;
		data.currentTile = &currentTile;
			
		threads.push_back(std::thread(RayThread, data));
	}
	for (int i = 0; i < global_options.num_threads; i++)
	{
		threads[i].join();
	}
}

void RayTracer::RenderToFile(const char* filename, int width, int height)
{
	unsigned char* pixels = new unsigned char[width * height * 3];
	double* fpixels = new double[width * height * 3];

	RenderToPixels(fpixels, width, height);
	for (int i = 0; i < width * height * 3; i++)
	{
		pixels[i] = static_cast<int>(fpixels[i] * 255);
	}
	stbi_write_png(filename, width, height, 3, pixels, sizeof(unsigned char) * 3 * width);
}

Intersection RayTracer::IntersectWithScene(const Ray& ray) const
{
	IObject* bestObject = nullptr;
	Intersection bestIntersection;
	bestIntersection.hitObject = nullptr;

#if 1
	bestObject = scene_->GetOctTree()->Traverse(ray, bestIntersection, T_MIN, T_MAX);
#else

	int limit = int(Objects_.size());
	for (int i = 0; i < limit; i++)
	{
		if (i == from)
			continue;

		Intersection intersection;
		if (Objects_[i]->Intersect(ray, intersection))
		{
			if (bestObject == nullptr || intersection.distance < bestIntersection.distance)
			{
				bestObject = Objects_[i];
				bestIntersection = intersection;
				bestIntersection.object = i;
			}
		}
	}
#endif
	bestIntersection.incidentRay = ray;
	return bestIntersection;
}

Color RayTracer::SampleScene(const Ray& ray)
{
	// Camera rays are primary rays, and if they hit a light source, they should measure the emittance.
	return Trace(ray, 0, nullptr, true, true);
}

// sourceObject is set when tracing a ray transmitted through a material
// isPrimaryRay is used for camera rays and specular reflections/transmissions, used to determine whether to use the photon map to estimate radiance.
// sampleEmittance tells the ray tracer to include emitted light from a hit material.
// Used when tracing a specular reflection or camera ray.  Because direct lighting is done for diffuse materials, we exclude emittance from diffuse rays.
Color RayTracer::Trace(const Ray& ray, int depth, IObject* sourceObject, bool isPrimaryRay, bool sampleEmittance)
{
	// Lighting is calculated from 5 components
	// 1. Direct illumination.
	// 2. Specular reflection/transmission
	// 3. Caustics
	// 4. Multiple diffuse reflections
	// 5. Emittance
	if (depth > global_options.max_ray_depth)
	{
		return Color(0, 0, 0);
	}

	Intersection intersection = IntersectWithScene(ray);
	intersection.sourceObject = sourceObject;
	if (intersection.hitObject == nullptr)
	{
		return scene_->GetBackgroundColor();
	}

	Vector3 point = intersection.point;
	double t = (point - ray.origin).Magnitude();
	Vector3 normal = intersection.normal;
	const Material* material = intersection.hitObject->GetMaterial();

	if (global_options.use_photon_mapping && global_options.use_caustics && global_options.debug_caustics_photon_map && material->HasDiffuseComponent())
	{
		Color photonColor = scene_->GetCausticPhotonMap()->GetLuminance(*material, ray, intersection, global_options.caustics_photon_map_sample_count);
		return photonColor;
	}

	if (global_options.use_photon_mapping && (!isPrimaryRay || global_options.use_photon_map_approximation) && material->HasDiffuseComponent())
	{
		// evaluate global photon map for approximation at this point
		Color photonColor = scene_->GetPhotonMap()->GetLuminance(*material, ray, intersection, global_options.global_photon_map_sample_count);
		return photonColor;
	}

	// 1. Direct illumination
	Color directIllumination = CalculateDirectIllumination(intersection, material);

	// 2. Specular/glossy reflection/transmission
	Color specularReflection(0, 0, 0);
	Color specularTransmission(0, 0, 0);
	Color diffuseReflection(0, 0, 0);
	// 4. Diffuse reflection

	bool scatterDiffuse = false;
	bool scatterSpecular = false;
	bool scatterTransmission = false;
	bool adjustScatterProbability = false;
	// If using distribution tracing, select only one scatter type to avoid exponential recursive ray growth.  Otherwise, use all applicable scatter types and adjust the result by the probability of that type.
	if (global_options.use_distribution_tracing)
	{
		ScatterType scatterType = material->ScatterRussianRoulette(intersection, false);
		if (scatterType == ScatterType::Diffuse)
		{
			scatterDiffuse = true;
		}
		else if (scatterType == ScatterType::Specular)
		{
			scatterSpecular = true;
		}
		else if (scatterType == ScatterType::Transmission)
		{
			scatterTransmission = true;
		}
	}
	else
	{
		adjustScatterProbability = true;
		if (material->HasSpecularComponent())
		{
			scatterSpecular = true;
		}
		if (material->HasTransmissionComponent())
		{
			scatterTransmission = true;
		}
		// No diffuse scattering happens without distribution tracing
	}

	if (scatterSpecular)
	{
		specularReflection = CalculateSpecular(intersection, material, depth, sourceObject, isPrimaryRay, adjustScatterProbability);
	}

	if (scatterTransmission)
	{
		specularTransmission = CalculateTransmission(intersection, material, depth, sourceObject, isPrimaryRay, adjustScatterProbability);
	}
	
	if (scatterDiffuse)
	{
		diffuseReflection = CalculateDiffuse(intersection, material, depth, sourceObject, isPrimaryRay, adjustScatterProbability);
	}

	// 3. Caustics
	Color causticLuminance(0, 0, 0);
	if (global_options.use_photon_mapping && global_options.use_caustics)
	{
		causticLuminance = CalculateCaustics(intersection, material, ray);
	}	
	
	// 5. Emittance
	// Because we directly sample lights for diffuse materials, skip emittance in those cases, otherwise it will be counted twice.
	Color emittance(0, 0, 0);
	if (sampleEmittance)
	{
		emittance = CalculateEmittance(intersection, material);
	}

	Color finalColor = directIllumination + specularReflection + specularTransmission + causticLuminance + diffuseReflection + emittance;
	return finalColor;
}


Color RayTracer::CalculateDiffuse(const Intersection& intersection, const Material* material, int depth, IObject* sourceObject, bool isPrimaryRay, bool adjustScatterProbablility)
{
	Color diffuseReflection(0, 0, 0);
	ScatterResult result;
	if (material->Scatter(intersection, ScatterType::Diffuse, result))
	{
		Ray scattered;
		bool importanceSampling = false; // enable if samplingPdf != materialPdf

		Pdf* materialPdf = result.pdf.get();
		Pdf* samplingPdf = result.pdf.get(); // replace with different pdf for importance sampling
		scattered.origin = intersection.point;
		scattered.direction = samplingPdf->GenerateRandom();

		double pdfAdjustment = 1.0;
		if (importanceSampling)
		{
			double samplingPdfValue = samplingPdf->Value(scattered.direction);
			pdfAdjustment = (materialPdf->Value(scattered.direction) / samplingPdfValue);
		}

		Color scatteredDiffuseColor = Trace(scattered, depth + 1, sourceObject, false, false);
		diffuseReflection = result.Attenuation * scatteredDiffuseColor * pdfAdjustment;
		if (adjustScatterProbablility)
		{
			diffuseReflection *= result.resultProbability;
		}
	}
	return diffuseReflection;
}

Color RayTracer::CalculateSpecular(const Intersection& intersection, const Material* material, int depth, IObject* sourceObject, bool isPrimaryRay, bool adjustScatterProbablility)
{
	Color specularReflection(0, 0, 0);
	ScatterResult result;
	if (material->Scatter(intersection, ScatterType::Specular, result))
	{
		specularReflection = result.Attenuation * Trace(result.SpecularRay, depth + 1, sourceObject, isPrimaryRay, true);
		if (adjustScatterProbablility)
		{
			specularReflection *= result.resultProbability;
		}
	}
	return specularReflection;
}

Color RayTracer::CalculateTransmission(const Intersection& intersection, const Material* material, int depth, IObject* sourceObject, bool isPrimaryRay, bool adjustScatterProbablility)
{
	Color specularTransmission(0, 0, 0);

	ScatterResult result;
	if (material->Scatter(intersection, ScatterType::Transmission, result))
	{
		IObject* newSourceObject = sourceObject;
		if (intersection.internalIntersection)
		{
			newSourceObject = nullptr; // todo - supported nested object transmission.
		}
		else
		{
			newSourceObject = intersection.hitObject;
		}
		specularTransmission = result.Attenuation * Trace(result.SpecularRay, depth + 1, newSourceObject, isPrimaryRay, true);
		if (adjustScatterProbablility)
		{
			specularTransmission *= result.resultProbability;
		}
	}
	return specularTransmission;
}

Color RayTracer::CalculateCaustics(const Intersection& intersection, const Material* material, const Ray& ray)
{
	Color photonColor = scene_->GetCausticPhotonMap()->GetLuminance(*material, ray, intersection, global_options.caustics_photon_map_sample_count);
	return photonColor;
}

// a. For an area light source, Ldirect(shadowRay) = Le(shadowRay) * brdf(ray, shadowRay) * radianceTransfer / pdf(shadowIntersection.point)
// b. For a point light source, G omits the cosine of the shadowRay/light, and pdf is 1.
Color RayTracer::CalculateDirectIllumination(const Intersection& intersection, const Material* material)
{
	Color directIllumination(0, 0, 0);
	for (auto& light : scene_->GetLights())
	{
		Vector3 sampleDirection;
		double samplePdf;
		Color lightEmittance = light->SampleLightEmittance(intersection, sampleDirection, samplePdf);

		Ray shadowRay;
		shadowRay.origin = intersection.point;
		shadowRay.direction = sampleDirection;

		Intersection shadowIntersection = IntersectWithScene(shadowRay);

		if (light->IsOccluded(intersection, shadowIntersection))
		{
			continue;
		}

		Color brdfValue = material->BrdfValue(intersection, sampleDirection);
		double radianceTransfer = light->EvaluateRadianceTransfer(intersection, shadowIntersection);
		Color illumination = lightEmittance * brdfValue * radianceTransfer / samplePdf;
		directIllumination += illumination;
	}
	return directIllumination;
}

Color RayTracer::CalculateEmittance(const Intersection& intersection, const Material* material)
{
	return material->Emit(intersection);
}