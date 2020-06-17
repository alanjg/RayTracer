#include "pch.h"
#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(const Vector3& radii) :
	radii_(radii)
{

}

bool Ellipsoid::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	//Solve parametrically
	//ray: x=x0+at,y=y0+bt,z=z0+ct
	//(a,b,c)=theray.direction, (x0,y0,z0)=theray.origin
	//Ellipsoid: x*x/rx*rx+y*y/ry*ry+z*z/rz*rz=1, set equal, solve for t
	//imginary roots => no intersection
	//1 root => intersection on surface
	//2 roots => find closest to origin

	Vector3 origin = ray.origin;
	Vector3 direction = ray.direction;
	Vector3 scaledOrigin(origin);
	Vector3 scaledDirection(direction);
	scaledOrigin[0] /= radii_[0];
	scaledOrigin[1] /= radii_[1];
	scaledOrigin[2] /= radii_[2];
	scaledDirection[0] /= radii_[0];
	scaledDirection[1] /= radii_[1];
	scaledDirection[2] /= radii_[2];

	double a = scaledDirection.Dot(scaledDirection);
	double b = 2.0 * scaledDirection.Dot(scaledOrigin);
	double c = scaledOrigin.Dot(scaledOrigin) - 1;
	double disc = b * b - 4 * a * c;
	if (disc < 0.0)
	{
		return false;
	}

	disc = sqrt(disc);
	double t1 = (-b - disc) / (2 * a);
	if (t1 > 0.0 && t1 > tMin && t1 < tMax)
	{
		Vector3 point = ray.origin + t1 * ray.direction;
		Vector3 normal = point;
		normal[0] /= radii_[0] * radii_[0];
		normal[1] /= radii_[1] * radii_[1];
		normal[2] /= radii_[2] * radii_[2];
		normal.Normalize();

		Intersection intersection;
		intersection.point = point;
		intersection.normal = normal;
		intersection.distance = t1;
		// ray originated inside the ellipsoid, reverse the normal to point inward
		if (c < INTERSECTION_ADJUSTMENT)
		{
			normal *= -1;
			intersection.internalIntersection = true;
		}
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

	double t2 = (-b + disc) / (2 * a);
	if (t2 > 0.0 && (!firstPointOnly || t2 < firstIntersection.distance) && t2 > tMin && t2 < tMax)
	{
		Vector3 point = ray.origin + t2 * ray.direction;
		Vector3 normal = point;
		normal[0] /= radii_[0] * radii_[0];
		normal[1] /= radii_[1] * radii_[1];
		normal[2] /= radii_[2] * radii_[2];
		normal.Normalize();

		Intersection intersection;
		intersection.point = point;
		intersection.normal = normal;
		intersection.distance = t2;
		// ray originated inside the ellipsoid, reverse the normal to point inward
		if (c < INTERSECTION_ADJUSTMENT)
		{
			normal *= -1;
			intersection.internalIntersection = true;
		}
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
	return intersectionPoints.size() > 0;
}

Vector3 Ellipsoid::GetMin() const
{
	return -radii_;
}

Vector3 Ellipsoid::GetMax() const
{
	return radii_;
}