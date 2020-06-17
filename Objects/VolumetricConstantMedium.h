#pragma once
#include "../Object.h"
#include "../Ray.h"
#include "../Texture.h"
#include "../Transform.h"
#include "../Intersection.h"
#include "../Vector.h"

class VolumetricConstantMedium : public Object
{
public:
	VolumetricConstantMedium(Shape* boundary, double density, const Transform& transform, Texture* albedo);
	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	~VolumetricConstantMedium();

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

private:
	Shape* boundary_;
	double density_;
};