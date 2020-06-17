#pragma once
#include "../Materials/Material.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"

class Cylinder : public Shape
{
public:
	Cylinder(double radius, double height);
	bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	
	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
private:
	double radius_;
	double height_;
};
