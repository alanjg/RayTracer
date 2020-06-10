#include "pch.h"
#include "Object.h"

bool Shape::Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const
{
	std::vector<Intersection> intersectionPoints;
	return GetIntersectionPoints(ray, intersectionPoints, intersection, tMin, tMax, true);

}

void Shape::GetPoints(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const
{
	Intersection intersection;
	GetIntersectionPoints(ray, points, intersection, tMin, tMax, false);
}

Object::~Object() 
{
}

bool Object::Intersect(const Ray& r, Intersection& intersection, double tMin, double tMax) const
{
	std::vector<Intersection> intersectionPoints;
	return GetIntersectionPoints(r, intersectionPoints, intersection, tMin, tMax, true);

}

void Object::GetPoints(const Ray& r, std::vector<Intersection>& points, double tMin, double tMax) const
{
	Intersection intersection;
	GetIntersectionPoints(r, points, intersection, tMin, tMax, false);
}

bool ShapeObject::Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const
{
	Ray localRay = transform_.TransformWorldToLocal(ray);
	bool hit = shape_->Intersect(localRay, intersection, tMin, tMax);
	if (hit)
	{
		Vector3 worldPoint = transform_.TransformLocalPointToWorld(intersection.point);
		Vector3 worldNormal = transform_.TransformLocalNormalToWorld(intersection.point);
		intersection.point = worldPoint;
		intersection.normal = worldNormal;
		intersection.intersectionTime = (ray.origin - worldPoint).Magnitude();
	}
	return hit;
}

void ShapeObject::GetPoints(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const
{
	Ray localRay = transform_.TransformWorldToLocal(ray);
	shape_->GetPoints(localRay, points, tMin, tMax);
	for (unsigned int i = 0; i < points.size(); i++)
	{
		Vector3 worldPoint = transform_.TransformLocalPointToWorld(points[i].point);
		Vector3 worldNormal = transform_.TransformLocalNormalToWorld(points[i].point);
		points[i].point = worldPoint;
		points[i].normal = worldNormal;
		points[i].intersectionTime = (ray.origin - worldPoint).Magnitude();
	}
}

Vector3 ShapeObject::GetMin() const
{
	return transform_.TransformLocalPointToWorld(shape_->GetMin());
}

Vector3 ShapeObject::GetMax() const
{
	return transform_.TransformLocalPointToWorld(shape_->GetMax());
}