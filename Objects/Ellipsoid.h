#pragma once
#include "../Materials/Material.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"

class Ellipsoid : public Object
{
public:
	Ellipsoid(const Vector3& radii, const Transform& transform, const Material* material);
	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool Contains(const Vector3& point);
	~Ellipsoid();

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
private:
	Vector3 radii_;
};