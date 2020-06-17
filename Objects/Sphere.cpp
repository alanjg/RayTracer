#include "pch.h"
#include "Sphere.h"

Sphere::Sphere(double radius)
{
	radius_ = radius;
}

bool Sphere::GetRoot(const Ray& ray, double a, double b, double c, double disc, double tMin, double tMax, Intersection& intersection) const
{
	double t = (-b - disc) / (2 * a);
	if (t > tMin && t < tMax)
	{
		intersection.point = ray.origin + t * ray.direction;
		intersection.normal = intersection.point / radius_;
		intersection.distance = t;
		// ray originated inside the sphere, reverse the normal to point inward
		if (LessThan(c, 0, INTERSECTION_ADJUSTMENT))
		{
			intersection.normal *= -1;
			intersection.internalIntersection = true;
		}
		return true;
	}
	return false;
}

bool Sphere::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	// Solve parametrically
	// ray: x=x0+at,y=y0+bt,z=z0+ct
	// (a,b,c)=theray.direction, (x0,y0,z0)=theray.origin
	// sphere: x*x+y*y+z*z=r*r, set equal, solve for t
	// imginary roots => no intersection
	// 1 root => intersection on surface
	// 2 roots => find closest to origin
	Vector3 origin = ray.origin;
	Vector3 direction = ray.direction;
	double a = direction.Dot(direction);
	double b = 2.0 * direction.Dot(origin);
	double c = origin.Dot(origin) - radius_ * radius_;
	double discSquared = b * b - 4 * a * c;

	if (discSquared < 0.0)
	{
		return false;
	}

	double disc = sqrt(discSquared);

	Intersection i1, i2;
	bool b1, b2;
	b1 = GetRoot(ray, a, b, c, disc, tMin, tMax, i1);
	b2 = GetRoot(ray, a, b, c, -disc, tMin, tMax, i2);
	
	if (!b1 && !b2)
	{
		return false;
	}

	if (b1 && (!b2 || i1.distance < i2.distance))
	{
		firstIntersection = i1;
	}
	else
	{
		firstIntersection = i2;
	}

	if (!firstPointOnly)
	{
		if (b1)
		{
			intersectionPoints.push_back(i1);
		}
		if (b2)
		{
			intersectionPoints.push_back(i2);
		}
	}
	return true;
}

Vector3 Sphere::GetMin() const
{
	return Vector3(-radius_, -radius_, -radius_);
}

Vector3 Sphere::GetMax() const
{
	return Vector3(radius_, radius_, radius_);
}