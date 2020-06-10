#pragma once
#include "../Materials/Material.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"

class Cylinder : public Object
{
public:
	Cylinder(double radius,double height, const Transform& transform, const Material* material);
	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool Contains(const Vector3& point);
	~Cylinder();

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
private:
	double radius_;
	double height_;
};
