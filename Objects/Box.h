#pragma once
#include "../Intersection.h"
#include "../Object.h"
#include "../Ray.h"
#include "../Transform.h"
#include "../Box.h"

class Box : public Object
{
public:
	Box(Box3 bounds, const Transform& transform, const Material* material);
	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool Contains(const Vector3& point);
	~Box();

	Vector3 GetMin() const;
	Vector3 GetMax() const;
private:
	Box3 bounds_;
};
