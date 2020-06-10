#include "pch.h"
#include "CSGObject.h"
#include "CSGNode.h"

CSGObject::CSGObject(CSGNode* tree, const Transform& transform, const Material* material): 
	Object(transform, material), tree_(tree)
{
	
}

//bool CSGObject::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly)
//{
//	if (firstPointOnly)
//	{
//		std::vector<Intersection> points;
//		tree_->GetPoints(ray, points, tMin, tMax);
//
//		if (points.empty())
//			return false;
//
//		firstIntersection = *std::min_element(points.begin(), points.end());
//		return true;
//	}
//	else
//	{
//		tree_->GetPoints(ray, intersectionPoints, tMin, tMax);
//		return intersectionPoints.size() > 0;
//	}
//}

bool CSGObject::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	
	if (firstPointOnly)
	{
		std::vector<Intersection> points;
		tree_->GetPointsEx(ray, points, tMin, tMax);

		if (points.empty())
		{
			return false;
		}

		firstIntersection = *std::min_element(points.begin(), points.end());
		return true;
	}
	else
	{
		tree_->GetPoints(ray, intersectionPoints, tMin, tMax);
		return intersectionPoints.size() > 0;
	}
}

bool CSGObject::Contains(const Vector3& point)
{
	return tree_->Contains(point);
}

Vector3 CSGObject::GetMin() const
{
	return tree_->GetMin();
}

Vector3 CSGObject::GetMax() const
{
	return tree_->GetMax();
}
