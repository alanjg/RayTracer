#pragma once
#include "../Intersection.h"
#include "../Object.h"
#include "../Ray.h"
#include "../Transform.h"
#include "../Box.h"

class Box : public Shape
{
public:
	Box(Box3 bounds);
	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

private:
	virtual bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	Box3 bounds_;
};
