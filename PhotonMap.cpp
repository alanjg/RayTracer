#include "pch.h"
#include "PhotonMap.h"


Color EstimateRadiance(const std::vector<Photon>& searchResults, const Intersection& intersection, const Material& material, double maxActualDistance);
Color EstimateRadianceConeFilter(const std::vector<Photon>& searchResults, const Intersection& intersection, const Material& material, double maxActualDistance);

PhotonMap::PhotonMap()
	:kdtree(nullptr)
{
}

void PhotonMap::Populate(const OctTree& tree, const std::vector<std::unique_ptr<Light>>& lights, const std::vector<std::unique_ptr<IObject>>& objects, bool caustics, int photonCount)
{
	unsigned int percent = 0;
	int emittedPhotons = 0;
	std::cerr << "Populating photon map." << std::endl;
	photons.reserve(photonCount);
	std::vector<int> photonsPerLight;
	double totalPower = 0;
	for (auto& light : lights)
	{
		Color power = light->Power();
		totalPower += power.r + power.g + power.b;
	}
	for (auto& light : lights)
	{
		Color power = light->Power();
		photonsPerLight.push_back((int)(photonCount * (power.r + power.g + power.b) / totalPower));
	}
	for (unsigned int lightIndex = 0; lightIndex < lights.size(); lightIndex++)
	{
		int currentLightPhotonCount = 0;
		while (currentLightPhotonCount < photonCount)
		{
			if (global_options.debugOutputLevel >= DebugOutputLevel::DEBUG_OUTPUT_PROGRESS)
			{
				if (100 * photons.size() / photonCount > percent)
				{
					percent = 100 * photons.size() / photonCount;
					std::cerr << percent << "%" << std::endl;
				}
			}

			Light& light = *lights[lightIndex];
			Ray nextRay;
			Photon photon = light.GenerateRandomPhoton(nextRay.direction);
			emittedPhotons++;
			nextRay.direction.Normalize();
			bool firstPass = true;
			nextRay.origin = photon.position;
			IObject* sourceObject = nullptr; // used to track materials for transmission
			for (;;)
			{
				Ray ray = nextRay;
				Intersection intersection;
				IObject* hitObject = tree.Traverse(ray, intersection, T_MIN, T_MAX);
				intersection.incidentRay = ray;
				intersection.sourceObject = sourceObject;
				if (hitObject == nullptr)
				{
					//we didn't hit anything, break
					break;
				}
				else
				{
					photon.position = intersection.point;
					photon.direction = ray.direction;
					//photon.SetIncomingDirection(intersection, ray.direction)

					IObject& object = *hitObject;
					const Material* material = object.GetMaterial();

					// If a diffuse object is hit, store the photon
					if (material->HasDiffuseComponent())
					{
						// caustics photon map should not store photon without a specular reflection/transmission
						if (!(caustics && firstPass))
						{
							// Make a copy of the photon
							photons.push_back(photon);
							currentLightPhotonCount++;
						}
					}
					firstPass = false;

					ScatterType type = material->ScatterRussianRoulette(intersection, true);
					if (type == ScatterType::None)
					{
						// absorption, terminate the photon
						break;
					}

					else if (type == ScatterType::Diffuse)
					{
						// Caustics photon map should never follow a diffuse reflection of the photon.
						if (caustics)
						{							
							break;
						}

						ScatterResult result;
						if (material->Scatter(intersection, type, result))
						{
							photon.ScalePower(result.Attenuation);

							nextRay.origin = intersection.point;
							nextRay.direction = result.pdf->GenerateRandom();
							nextRay.direction.Normalize();
						}
						else
						{
							// absorption
							break;
						}
					}
					else if (type == ScatterType::Specular || type == ScatterType::Transmission)
					{
						ScatterResult result;
						if (material->Scatter(intersection, type, result))
						{
							if (type == ScatterType::Transmission)
							{
								// todo - support nested objects
								if (intersection.internalIntersection)
								{
									sourceObject = nullptr;
								}
								else
								{
									sourceObject = intersection.hitObject;
								}
							}
							photon.ScalePower(result.Attenuation);
							nextRay = result.SpecularRay;
						}
						else
						{
							// absorption
							break;
						}
					}
				}
			}
		}
	}

	double scale = 1.0 / emittedPhotons;
	for (unsigned int i = 0; i < photons.size(); i++)
	{
		photons[i].power = photons[i].power * scale;
	}
	kdtree = new KDTree(photons);
}

Color PhotonMap::GetLuminance(const Material& material, const Ray& incoming, const Intersection& intersection, int numPhotons)
{
	std::vector<Photon> searchResults;
	int MIN_PHOTONS = 4;
	double maxSearchDistanceSquared = global_options.photon_map_max_sample_distance * global_options.photon_map_max_sample_distance;
	double maxActualDistance = kdtree->Search(intersection.point, numPhotons, maxSearchDistanceSquared, searchResults);

	int num = searchResults.size();
	if (num >= MIN_PHOTONS)
	{
		//return EstimateRadianceConeFilter(searchResults, intersection, material, maxActualDistance);
		return EstimateRadiance(searchResults, intersection, material, maxActualDistance);
	}
	return Color(0, 0, 0);
}

Color EstimateRadiance(const std::vector<Photon>& searchResults, const Intersection& intersection, const Material& material, double maxActualDistance)
{
	Color flux(0, 0, 0);
	int num = searchResults.size();
	for (unsigned int i = 0; i < searchResults.size(); i++)
	{
		const Photon& photon = searchResults[i];
		Vector3 photonOutgoingDir = -photon.direction; //IncomingDirection();

		// check photon dir against incoming direction to ensure it's the right side of the surface, needed for thin shapes
		if (photonOutgoingDir.Dot(intersection.normal) < 0)
		{
			// todo - Is this right for transmission?
			//continue;
		}

		Color brdfValue = material.BrdfValue(intersection, photonOutgoingDir);
		flux += photon.power * brdfValue;
	}
	
	Color radiance(0, 0, 0);
	radiance = flux * (1 / (2.0 * M_PI * maxActualDistance * maxActualDistance));
	return radiance;
}

Color EstimateRadianceConeFilter(const std::vector<Photon>& searchResults, const Intersection& intersection, const Material& material, double maxActualDistance)
{
	double k = 1.1;
	Color flux(0, 0, 0);
	int num = searchResults.size();
	for (unsigned int i = 0; i < searchResults.size(); i++)
	{
		const Photon& photon = searchResults[i];
		Vector3 photonOutgoingDir = -photon.direction; //IncomingDirection();

		// check photon dir against incoming direction to ensure it's the right side of the surface, needed for thin shapes
		if (photonOutgoingDir.Dot(intersection.normal) < 0)
		{
			// todo - Is this right for transmission?
			//continue;
		}

		Color brdfValue = material.BrdfValue(intersection, photonOutgoingDir);
		double w = 1 - (photon.position - intersection.point).Magnitude() / (k * maxActualDistance);
		flux += photon.power * brdfValue * w;
	}
	
	Color radiance(0, 0, 0);
	radiance = flux * (1 / (((1 - 2 / (3 * k)) * M_PI * maxActualDistance * maxActualDistance)));
	return radiance;
}

Color PhotonMap::GetDebugVisualization(const Material& material, const Ray& incoming, const Intersection& intersection)
{
	std::vector<Photon> searchResults;
	double maxSearchDistanceSquared = 4;
	double maxActualDistance = kdtree->Search(intersection.point, 6, maxSearchDistanceSquared, searchResults);
	int count = searchResults.size();
	if (count == 0)
	{
		return Color(0, 0, 0);
	}
	else if(count < 3)
	{
		return Color(0, 0.5*count, 0);
	}
	else if (count < 5)
	{
		return Color(0, 0, 0.5 * (count - 2));
	}
	else
	{
		return Color(0.5 * (count - 4), 0, 0);
	}
	
}