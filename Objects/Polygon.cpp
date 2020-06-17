#include "pch.h"
#include "Polygon.h"

bool PointInPolygon(const std::vector<Vector3>& vertices, const Vector3& point)
{
	double angleSum = 0;

	int n = static_cast<int>(vertices.size());

	for(int i=0; i<n; i++)
	{
		Vector3 a = vertices[i]-point;
		Vector3 b = vertices[(i+1)%n]-point;
		double den = a.Magnitude()*b.Magnitude();
		if(fabs(den) < INTERSECTION_ADJUSTMENT)
			return true;

		double cosTheta = a.Dot(b) / den;
		cosTheta = std::min(std::max(cosTheta,-1.0),1.0);
		angleSum += acos(cosTheta);
	}

	return fabs(angleSum-2*3.14159) <= INTERSECTION_ADJUSTMENT;
}

Polygon::Polygon(const std::vector<Vector3>& points) :
	points_(points)
{
	normal_ = (points[2] - points[1]).Cross(points[0] - points[1]);
	normal_.Normalize();
	planeDistance_ = -normal_.Dot(points[0]);
}

Vector3 Polygon::GetNormal()
{
	return normal_;
}

bool Polygon::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	double den = -normal_.Dot(ray.direction);
	if (den == 0)
	{
		return false;
	}

	double t = (planeDistance_ + normal_.Dot(ray.origin)) / den;
	if (t <= 0.0)
	{
		return false;
	}

	Vector3 point = ray.origin + ray.direction * t;
	if (!PointInPolygon(points_, point))
	{
		return false;
	}

	if (t > tMin && t < tMax)
	{
		Intersection intersection;
		intersection.point = point;
		intersection.normal = normal_;
		intersection.distance = t;
		if (normal_.Dot(ray.direction) > 0)
		{
			intersection.internalIntersection = true;
			intersection.normal *= -1;
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
	return false;
}

Vector3 Polygon::GetMin() const
{
	Vector3 vmin = points_[0];
	for (unsigned int i = 1; i < points_.size(); i++)
		vmin = VMin(vmin, points_[i]);
	return vmin;
}

Vector3 Polygon::GetMax() const
{
	Vector3 vmax = points_[0];
	for (unsigned int i = 1; i < points_.size(); i++)
		vmax = VMax(vmax, points_[i]);
	return vmax;
}