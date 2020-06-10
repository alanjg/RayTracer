#include "pch.h"
#include "Scene.h"
#include "Materials/Material.h"
#include "Materials/MaterialManager.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/DielectricMaterial.h"
#include "Materials/MetalMaterial.h"
#include "Materials/IsotropicMaterial.h"
#include "ObjectFactory.h"

void Scene::AddObject(IObject* object)
{
	objects_.push_back(std::unique_ptr<IObject>(object));
}

void Scene::AddLight(Light* light)
{
	lights_.push_back(std::unique_ptr<Light>(light));
}

void Scene::AddObject(std::unique_ptr<IObject> object)
{
	objects_.push_back(std::move(object));
}

void Scene::AddLight(std::unique_ptr<Light> light)
{
	lights_.push_back(std::move(light));
}

std::vector<std::unique_ptr<Light>>& Scene::GetLights()
{
	return lights_;
}

std::vector<std::unique_ptr<IObject>>& Scene::GetObjects()
{
	return objects_;
}

void Scene::BalanceTree()
{
	tree_->Balance();
	tree_->PrintTree();
	if (global_options.use_photon_mapping)
	{
		photonMap_.reset(new PhotonMap());
		photonMap_->Populate(*tree_, lights_, objects_, false, global_options.global_photon_map_photon_count);

		if (global_options.use_caustics)
		{
			causticPhotonMap_.reset(new PhotonMap());
			causticPhotonMap_->Populate(*tree_, lights_, objects_, true, global_options.caustics_photon_map_photon_count);
		}
	}
}

Scene::Scene()
{
	tree_.reset(new OctTree(objects_));
	photonMap_ = 0;
	causticPhotonMap_ = 0;
}

Scene::~Scene()
{
}

void Scene::SetBackgroundColor(Color color)
{
	backgroundColor_ = color;
}

Camera& Scene::GetCamera()
{
	return camera_;
}

Color Scene::GetBackgroundColor() const
{
	return backgroundColor_;
}

OctTree* Scene::GetOctTree() const
{
	return tree_.get();
}


PhotonMap* Scene::GetPhotonMap() const
{
	return photonMap_.get();
}

PhotonMap* Scene::GetCausticPhotonMap() const
{
	return causticPhotonMap_.get();
}


Light* MakePointLight(std::istream& in, Scene& scene)
{
	Vector3 position;
	Color power, emittance;

	std::string s;
	in >> s;
	std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)tolower(c); });
	while (s != "}")
	{
		if (s == "position")
		{
			in >> position;
		}
		else if (s == "power")
		{
			in >> power;
		}
		else if (s == "emittance")
		{
			in >> emittance;
		}
		in >> s;
	}
	Light* light = new PointLight(position, power, emittance);
	return light;
}

Light* MakeAndAddRectangleAreaLight(std::istream& in, Scene& scene)
{
	Vector3 position, ll, lr, ul;
	Color power;
	Color emittance;

	std::string s;
	in >> s;
	std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)tolower(c); });
	while (s != "}")
	{
		if (s == "ll")
		{
			in >> ll;
		}
		else if (s == "ul")
		{
			in >> ul;
		}
		else if (s == "lr")
		{
			in >> lr;
		}
		else if (s == "power")
		{
			in >> power;
		}
		else if (s == "emittance")
		{
			in >> emittance;
		}
		in >> s;
	}
	DiffuseLightMaterial* material = new DiffuseLightMaterial(emittance);
	g_MaterialManager.AddUntracked(material);
	Transform transform;
	
	auto ret = CreateRectangleAreaLight(ll, lr, ul, power, emittance, transform, material);
	Light* light = ret.first.get();
	scene.AddLight(std::move(ret.first));
	scene.AddObject(std::move(ret.second));
	return light;
}

Material* MakeLambertianMaterial(std::istream& in)
{
	Color albedo(0,0,0);
	std::string s;
	in >> s;
	while (s != "}")
	{
		transform(s.begin(), s.end(), s.begin(), tolower);
		if (s == "albedo")
		{
			in >> albedo;
		}
		in >> s;
	}
	Material* material = new LambertianMaterial(albedo);
	return material;
}

Material* MakeMetalMaterial(std::istream& in)
{
	Color albedo(0,0,0);
	double fuzziness = 0;
	std::string s;
	in >> s;
	while (s != "}")
	{
		transform(s.begin(), s.end(), s.begin(), tolower);
		if (s == "albedo")
		{
			in >> albedo;
		}
		else if (s == "fuzziness")
		{
			in >> fuzziness;
		}
		in >> s;
	}
	Material* material = new MetalMaterial(albedo, fuzziness);
	return material;
}

Material* MakeIsotropicMaterial(std::istream& in)
{
	Color albedo(0, 0, 0);
	std::string s;
	in >> s;
	while (s != "}")
	{
		transform(s.begin(), s.end(), s.begin(), tolower);
		if (s == "albedo")
		{
			in >> albedo;
		}
		in >> s;
	}
	Material* material = new IsotropicMaterial(new ConstantTexture(albedo));
	return material;
}

Material* MakeDielectricMaterial(std::istream& in)
{
	double ri = 0;
	std::string s;
	in >> s;
	while (s != "}")
	{
		transform(s.begin(), s.end(), s.begin(), tolower);
		if (s == "ri")
		{
			in >> ri;
		}
		in >> s;
	}
	Material* material = new DielectricMaterial(ri);
	return material;
}

std::istream& operator>>(std::istream& in, Scene& scene)
{
	
	scene.lights_.clear();
	scene.objects_.clear();
	while (in)
	{
		std::string s;
		in >> s;
		std::transform(s.begin(), s.end(), s.begin(), [](char c) { return (char)tolower(c); });
		if (s == "camera")
		{
			Vector3 eye, coi, up;
			in >> eye >> coi >> up;

			up.Normalize();

			Vector3 dir = coi - eye;
			dir.Normalize();

			Vector3 right = dir.Cross(up);
			right.Normalize();

			up = right.Cross(dir);
			up.Normalize();

			scene.camera_.centerOfInterest = coi;
			scene.camera_.position = eye;
			scene.camera_.up = up;

			double fov;
			in >> fov;
			scene.camera_.fov = fov;
		}
		else if (s == "pointlight")
		{
			//grab {
			char c;
			in >> c;
			scene.AddLight(MakePointLight(in, scene));
		}
		else if (s == "rectanglearealight")
		{
			//grab {
			char c;
			in >> c;
			MakeAndAddRectangleAreaLight(in, scene);
		}
		else if (s == "object")
		{
			scene.AddObject(MakeObject(in));
		}
		else if (s == "material")
		{
			std::string type;
			in >> type >> s;
			//grab {
			char c;
			in >> c;
			if (type == "LambertianMaterial")
			{
				g_MaterialManager.Add(s, MakeLambertianMaterial(in));
			}
			else if (type == "MetalMaterial")
			{
				g_MaterialManager.Add(s, MakeMetalMaterial(in));
			}
			else if (type == "DielectricMaterial")
			{
				g_MaterialManager.Add(s, MakeDielectricMaterial(in));
			}
			else if (type == "IsotropicMaterial")
			{
				g_MaterialManager.Add(s, MakeIsotropicMaterial(in));
			}
		}
		else if (s == "background")
		{
			in >> scene.backgroundColor_;
		}
		else
		{
			std::getline(in, s);
		}
	}
	
	return in;
}

std::ostream& operator<<(std::ostream& out, const Scene& scene)
{
	return out;
}
