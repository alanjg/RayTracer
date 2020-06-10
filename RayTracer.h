#pragma once
#include "Light.h"
#include "Object.h"
#include "Scene.h"

struct Tile
{
	int width;
	int height;
	int wBegin;
	int wEnd;
	int hBegin;
	int hEnd;
};

struct RayContext
{
	int depth;
	Color attenuation;
	Object* sourceObject;
	bool isPrimaryRay;
	bool isSpecularRay;
};

class RayTracer
{
public:
	RayTracer();
	~RayTracer();
	bool LoadScene(const std::string& filename);
	void RenderToPixels(double* pixels, int width, int height);
	void RenderToFile(const char* filename, int width, int height);
	void RenderTile(double* pixels, const Tile& tile);
	Scene* GetScene();

	Color SampleScene(const Ray& ray);	
	Intersection IntersectWithScene(const Ray& ray) const;
private:
	Color Trace(const Ray& ray, int depth, IObject* sourceObject, bool isPrimaryRay, bool sampleEmittance);

	Color CalculateDiffuse(const Intersection& intersection, const Material* material, int depth, IObject* sourceObject, bool isPrimaryRay, bool adjustScatterProbablility);
	Color CalculateSpecular(const Intersection& intersection, const Material* material, int depth, IObject* sourceObject, bool isPrimaryRay, bool adjustScatterProbablility);
	Color CalculateTransmission(const Intersection& intersection, const Material* material, int depth, IObject* sourceObject, bool isPrimaryRay, bool adjustScatterProbablility);
	Color CalculateCaustics(const Intersection& intersection, const Material* material, const Ray& ray);
	Color CalculateDirectIllumination(const Intersection& intersection, const Material* material);
	Color CalculateEmittance(const Intersection& intersection, const Material* material);

	std::stack<RayContext> contextStack;
	Scene* scene_;
};