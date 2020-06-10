#pragma once
#include "Camera.h"
#include "Color.h"
#include "Intersection.h"
#include "Light.h"
#include "Object.h"
#include "OctTree.h"
#include "PhotonMap.h"
#include "Ray.h"

class Scene
{
public:
	Scene();
	~Scene();

	friend std::istream& operator>>(std::istream& in, Scene& scene);
	friend std::ostream& operator<<(std::ostream& out, const Scene& scene);

	void AddObject(IObject* object);
	void AddLight(Light* light);
	void AddObject(std::unique_ptr<IObject> object);
	void AddLight(std::unique_ptr<Light> light);
	void BalanceTree();

	std::vector<std::unique_ptr<Light>>& GetLights();
	std::vector<std::unique_ptr<IObject>>& GetObjects();
	PhotonMap* GetPhotonMap() const;
	PhotonMap* GetCausticPhotonMap() const;
	OctTree* GetOctTree() const;
	Color GetBackgroundColor() const;
	
	Camera& GetCamera();
	void SetBackgroundColor(Color color);
private:
	std::unique_ptr<OctTree> tree_;
	std::unique_ptr<PhotonMap> photonMap_, causticPhotonMap_;
	std::vector<std::unique_ptr<IObject>> objects_;
	std::vector<std::unique_ptr<Light>> lights_;
	Camera camera_;
	Color backgroundColor_;
};

std::istream& operator>>(std::istream& in, Scene& scene);
std::ostream& operator<<(std::ostream& out, const Scene& scene);