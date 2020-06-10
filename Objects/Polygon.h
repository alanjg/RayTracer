#pragma once
#include "../Materials/Material.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"

class Polygon : public Object
{
public:
	Polygon(const std::vector<Vector3>& points, const Transform& transform, const Material* material);
	Vector3 GetNormal(); 
	bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool Contains(const Vector3& point);
	~Polygon();
	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
		
private:
	double planeDistance_;
	Vector3 normal_;
	std::vector<Vector3> points_;
};

class PolygonShape : public Shape
{
public:
	PolygonShape(const std::vector<Vector3>& points);
	~PolygonShape();

	Vector3 GetNormal();
	bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

private:
	double planeDistance_;
	Vector3 normal_;
	std::vector<Vector3> points_;
};
