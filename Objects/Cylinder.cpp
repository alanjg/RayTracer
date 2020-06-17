#include "pch.h"
#include "Cylinder.h"

Cylinder::Cylinder(double radius, double height) :
	radius_(radius), height_(height)
{
}

bool Cylinder::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	firstIntersection.distance = tMax;

	//solve for top & bottom
	if (ray.direction[1] != 0)
	{
		double top = (height_ - ray.origin[1]) / ray.direction[1];
		double bottom = -ray.origin[1] / ray.direction[1];

		if (top > 0.0 && (!firstPointOnly || top < firstIntersection.distance) && top > tMin && top < tMax)
		{
			Vector3 point = ray.origin + top * ray.direction;
			Vector3 normal(0, 1, 0);
			
			Intersection intersection;
			if (height_ - ray.origin[1])
			{
				intersection.internalIntersection = true;
				normal[1] = -1;
			}

			if (point[0] * point[0] + point[2] * point[2] <= radius_ * radius_)
			{
				intersection.point = point;
				intersection.normal = normal;
				intersection.distance = top;
				if (firstPointOnly)
				{
					firstIntersection = intersection;
				}
				else
				{
					intersectionPoints.push_back(intersection);
				}
			}
		}
		if (bottom > 0.0 && (!firstPointOnly || bottom < firstIntersection.distance) && bottom > tMin && bottom < tMax)
		{
			Vector3 point = ray.origin + bottom * ray.direction;
			Vector3 normal(0, -1, 0);

			Intersection intersection;
			intersection.internalIntersection = false;
			if (ray.origin[1] > 0)
			{
				intersection.internalIntersection = true;
				normal[1] = 1;
			}
			if (point[0] * point[0] + point[2] * point[2] <= radius_ * radius_)
			{
				intersection.point = point;
				intersection.normal = normal;
				intersection.distance = bottom;
				if (firstPointOnly)
				{
					firstIntersection = intersection;
				}
				else
				{
					intersectionPoints.push_back(intersection);
				}
			}
		}
	}

	// now check sides
	Vector3 direction = ray.direction;
	direction[1] = 0;
	Vector3 origin = ray.origin;
	origin[1] = 0;
	double a = direction.Dot(direction);
	double b = 2.0 * direction.Dot(origin);
	double c = origin.Dot(origin) - radius_ * radius_;
	double disc = b * b - 4 * a * c;

	if (disc < 0.0)
	{
		return firstIntersection.distance < tMax || intersectionPoints.size() > 0;
	}

	disc = sqrt(disc);
	double t1 = (-b - disc) / (2 * a);
	if (t1 > 0.0 && (!firstPointOnly || t1 < firstIntersection.distance) && t1 > tMin && t1 < tMax)
	{
		Intersection intersection;
		intersection.internalIntersection = false;
		Vector3 point = ray.origin + t1 * ray.direction;
		Vector3 normal = point - Vector3(0, point[1], 0);
		normal.Normalize();
		if (c < INTERSECTION_ADJUSTMENT)
		{
			intersection.internalIntersection = true;
			normal *= -1;
		}
		
		if (point[1] > 0 && point[1] < height_)
		{
			intersection.point = point;
			intersection.normal = normal;
			intersection.distance = t1;
			if (firstPointOnly)
			{
				firstIntersection = intersection;
				return true;
			}
			else
			{
				intersectionPoints.push_back(intersection);
			}

		}
	}

	double t2 = (-b + disc) / (2 * a);
	if (t2 > 0.0 && (!firstPointOnly || t2 < firstIntersection.distance) && t2 > tMin && t2 < tMax)
	{
		Intersection intersection;
		intersection.internalIntersection = false;
		Vector3 point = ray.origin + t2 * ray.direction;
		Vector3 normal = point - Vector3(0, point[1], 0);
		normal.Normalize();

		if (c < INTERSECTION_ADJUSTMENT)
		{
			intersection.internalIntersection = true;
			normal *= -1;
		}
		if (point[1] > 0 && point[1] < height_)
		{
			intersection.point = point;
			intersection.normal = normal;
			intersection.distance = t2;
			if (firstPointOnly)
			{
				firstIntersection = intersection;
				return true;
			}
			else
			{
				intersectionPoints.push_back(intersection);
			}

		}
	}
	return firstIntersection.distance != tMax || intersectionPoints.size() > 0;
}

Vector3 Cylinder::GetMin() const
{
	return Vector3(-radius_, 0, -radius_);;
}

Vector3 Cylinder::GetMax() const
{
	return Vector3(radius_, height_, radius_);;
}