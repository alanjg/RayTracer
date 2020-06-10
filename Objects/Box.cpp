#include "pch.h"
#include "Box.h"

Box::Box(Box3 bounds, const Transform& transform, const Material* material) :
	Object(transform, material), bounds_(bounds)
{
}

Box::~Box()
{

}

bool Box::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	
	return firstIntersection.intersectionTime < tMin || intersectionPoints.size() > 0;
}

bool Box::Contains(const Vector3& point)
{
	Vector3 localPoint = transform_.TransformWorldPointToLocal(point);
	return bounds_.Contains(localPoint);
}

Vector3 Box::GetMin() const
{
	return transform_.TransformLocalPointToWorld(bounds_.GetMin());
}

Vector3 Box::GetMax() const
{
	return transform_.TransformLocalPointToWorld(bounds_.GetMax());
}