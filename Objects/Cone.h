#pragma once
#include "../Intersection.h"
#include "../Object.h"
#include "../Ray.h"
#include "../Transform.h"


class Cone : public Object
{
public:
	Cone(double bottomRadius, double topRadius, double height, const Transform& transform, const Material* material);
	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool Contains(const Vector3& point);
	~Cone();

	Vector3 GetMin() const;
	Vector3 GetMax() const;
private:
	double bottomRadius_;
	double topRadius_;
	double height_;
};
