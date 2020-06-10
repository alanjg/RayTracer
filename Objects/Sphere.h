#pragma once
#include "../Math.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Materials/Material.h"
#include "../Ray.h"
#include "../Transform.h"
#include "../Vector.h"

class Sphere : public Object
{
public:
	Sphere(double radius, const Transform& transform, const Material* material);

	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool Contains(const Vector3& point);
	~Sphere();

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
private:
	double radius_;
	Vector3 vmin_, vmax_;
};

class SphereShape : public Shape
{
public:
	SphereShape(double radius);
	

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

private:
	virtual bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool GetRoot(const Ray& ray, double a, double b, double c, double disc, double tMin, double tMax, Intersection& intersection) const;
	double radius_;
};