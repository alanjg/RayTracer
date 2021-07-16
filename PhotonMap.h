#pragma once

#include "Color.h"
#include "KDTree.h"
#include "Light.h"
#include "Materials/Material.h"
#include "Object.h"
#include "OctTree.h"
#include "Photon.h"
#include "Vector.h"

class PhotonMap
{
	KDTree* kdtree;
	std::vector<Photon> photons;
public:
	PhotonMap();
	void Populate(const OctTree& tree, const std::vector<std::unique_ptr<Light>>& lights, const std::vector<std::unique_ptr<IObject>>& objects, bool caustics, int photonCount);
	Color GetLuminance(const Material& material, const Ray& incoming, const Intersection& intersection, int numPhotons);
	Color GetDebugVisualization(const Material& material, const Ray& incoming, const Intersection& intersection);
};
