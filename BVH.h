#pragma once
#include "Box.h"
#include "Intersection.h"
#include "IntersectionAccelerator.h"
#include "Object.h"
#include "Ray.h"

class BVHNode
{
private:
	Box3 bounds;
	BVHNode* left;
	BVHNode* right;
	IObject* object;
public:
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const;
};

class BVH : IntersectionAccelerator
{
	std::vector<std::unique_ptr<Object>>& objects_;
	BVHNode* root;
public:
	BVH(std::vector<std::unique_ptr<Object>>& objects);
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const = 0;
};