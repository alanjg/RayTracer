#include "pch.h"
#include "Box.h"

Box::Box(Box3 bounds) :
	bounds_(bounds)
{
}

bool Box::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{

	return firstIntersection.distance < tMin || intersectionPoints.size() > 0;
}

Vector3 Box::GetMin() const
{
	return bounds_.GetMin();
}

Vector3 Box::GetMax() const
{
	return bounds_.GetMax();
}