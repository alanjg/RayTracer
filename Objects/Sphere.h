#pragma once
#include "../Math.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Materials/Material.h"
#include "../Ray.h"
#include "../Transform.h"
#include "../Vector.h"

class Sphere : public Shape
{
public:
	Sphere(double radius);
	
	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

private:
	virtual bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool GetRoot(const Ray& ray, double a, double b, double c, double disc, double tMin, double tMax, Intersection& intersection) const;
	double radius_;
};