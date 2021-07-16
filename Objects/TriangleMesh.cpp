#include "pch.h"
#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(std::vector<Vector3> vertices, std::vector<int> indices) :
	vertices_(vertices), indices_(indices)
{
	bounds_.SetBounds(vertices[0], vertices[0]);
	for (const Vector3& v : vertices_)
	{
		bounds_.Extend(v);
	}
}

TriangleMesh::~TriangleMesh()
{
}

bool TriangleMesh::PointInTriangle(const Vector3& point, int faceIndex) const
{
	double angleSum = 0;

	for (int i = 0; i < 3; i++)
	{
		Vector3 a = vertices_[indices_[faceIndex + i]] - point;
		Vector3 b = vertices_[indices_[faceIndex + ((i + 1) % 3)]] - point;
		double den = a.Magnitude() * b.Magnitude();
		if (fabs(den) < INTERSECTION_ADJUSTMENT)
			return true;

		double cosTheta = a.Dot(b) / den;
		cosTheta = std::min(std::max(cosTheta, -1.0), 1.0);
		angleSum += acos(cosTheta);
	}

	return fabs(angleSum - 2 * 3.14159) <= INTERSECTION_ADJUSTMENT;
}

bool TriangleMesh::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	if (!bounds_.Intersect(ray, tMin, tMax)) 
	{ 
	//	return false; 
	}

	bool hitTriangle = false;
	for (size_t i = 0; i < indices_.size(); i+=3)
	{
		Vector3 p0 = vertices_[indices_[i+0]];
		Vector3 p1 = vertices_[indices_[i+1]];
		Vector3 p2 = vertices_[indices_[i+2]];

		Vector3 normal = (p2 - p1).Cross(p0 - p1);
		normal.Normalize();
		double planeDistance = -normal.Dot(p0);
		double den = -normal.Dot(ray.direction);
		if (den == 0)
		{
			continue;
		}

		double t = (planeDistance + normal.Dot(ray.origin)) / den;
		if (t <= 0.0)
		{
			continue;
		}

		Vector3 point = ray.origin + ray.direction * t;
		if (!PointInTriangle(point, i))
		{
			continue;
		}

		if (t > tMin && t < tMax)
		{			
			Intersection intersection;
			intersection.point = point;
			intersection.normal = normal;
			intersection.distance = t;
			if (normal.Dot(ray.direction) > 0)
			{
				intersection.internalIntersection = true;
				intersection.normal *= -1;
			}
			if (firstPointOnly && (!hitTriangle || intersection.distance < firstIntersection.distance))
			{
				firstIntersection = intersection;
			}
			else
			{
				intersectionPoints.push_back(intersection);
			}
			hitTriangle = true;
		}
	}
	return firstIntersection.distance > tMin || intersectionPoints.size() > 0;
}

Vector3 TriangleMesh::GetMin() const
{
	return bounds_.GetMin();
}

Vector3 TriangleMesh::GetMax() const
{
	return bounds_.GetMax();
}