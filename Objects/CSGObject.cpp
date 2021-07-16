#include "pch.h"
#include "CSGObject.h"
#include "CSGNode.h"

CSGShape::CSGShape(CSGNode* tree) :
	tree_(tree)
{

}

bool CSGShape::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{

	if (firstPointOnly)
	{
		std::vector<Intersection> points;
		tree_->GetPoints(ray, points, tMin, tMax);

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

Vector3 CSGShape::GetMin() const
{
	return tree_->GetMin();
}

Vector3 CSGShape::GetMax() const
{
	return tree_->GetMax();
}
