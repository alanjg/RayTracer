#pragma once
#include "../Intersection.h"
#include "../Object.h"
#include "../Ray.h"
#include "../Transform.h"

class Cone : public Shape
{
public:
	Cone(double bottomRadius, double topRadius, double height);
	
	Vector3 GetMin() const;
	Vector3 GetMax() const;
private:
	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;

	double bottomRadius_;
	double topRadius_;
	double height_;
};
