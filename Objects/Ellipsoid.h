#pragma once
#include "../Materials/Material.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"

class Ellipsoid : public Shape
{
public:
	Ellipsoid(const Vector3& radii);
	bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	
	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
private:
	Vector3 radii_;
};