#include "pch.h"
#include "BVH.h"

BVH::BVH(std::vector<std::unique_ptr<Object>>& objects)
	:objects_(objects), root(nullptr)
{
	
}

bool BVH::Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const
{
	return root->Intersect(ray, intersection, tMin, tMax);
}

bool BVHNode::Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const
{
	if (!bounds.Intersect(ray, tMin, tMax))
	{
		return false;
	}

	bool hitLeft = left->Intersect(ray, intersection, tMin, tMax);
	bool hitRight = right->Intersect(ray, intersection, tMin, hitLeft ? intersection.intersectionTime : tMax);
	return hitLeft || hitRight;
}