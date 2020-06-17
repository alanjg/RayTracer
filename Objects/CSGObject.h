#pragma once
#include "../Materials/Material.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"
#include "CSGNode.h"

//class CSGObject: public Object
//{
//public:
//	CSGObject(CSGNode* tree, const Transform& transform, const Material* material);
//	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
//
//	virtual Vector3 GetMin() const;
//	virtual Vector3 GetMax() const;
//
//private:
//	CSGNode* tree_;
//};

class CSGShape : public Shape
{
public:
	CSGShape(CSGNode* tree);
	bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

private:
	CSGNode* tree_;
};