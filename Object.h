#pragma once
#include "Color.h"
#include "Intersection.h"
#include "Light.h"
#include "Materials/Material.h"
#include "Ray.h"
#include "Transform.h"
#include "Vector.h"

class Shape
{
public:
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const;
	virtual void GetPoints(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const;

	virtual Vector3 GetMin() const = 0;
	virtual Vector3 GetMax() const = 0;

protected:
	virtual bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const = 0;
};

class IObject
{
public:
	virtual bool Intersect(const Ray& r, Intersection& intersection, double tMin, double tMax) const = 0;

	virtual Vector3 GetMin() const = 0;
	virtual Vector3 GetMax() const = 0;

	// used for CSG
	virtual void GetPoints(const Ray& r, std::vector<Intersection>& points, double tMin, double tMax) const = 0;
	virtual bool Contains(const Vector3& point) = 0;

	virtual const Material* GetMaterial() = 0;
};

class Object : public IObject
{
public:
	Object(const Transform& transform, const Material* material) :
		material_(material), transform_(transform), light_(nullptr)
	{
	}

	Object(const Transform& transform, const Material* material, const Light* light) :
		material_(material), transform_(transform), light_(light)
	{
	}

	virtual ~Object() = 0;
	const Material* GetMaterial() { return material_; }
	const Light* GetLight() { return light_; }

	// Evaluate the value of the pdf of a ray targeting the object from the given origin point and direction.
//	virtual double EvaluatePdf(const Vector3& origin, const Vector3& direction) = 0;

	// Generate a random ray
//	virtual Vector3 GenerateRandomRayToObject(const Vector3& origin) = 0;

	bool Intersect(const Ray& r, Intersection& intersection, double tMin, double tMax) const;
	void GetPoints(const Ray& r, std::vector<Intersection>& points, double tMin, double tMax) const;
	virtual bool Contains(const Vector3& point) = 0;
protected:
	const Material* material_;
	const Light* light_;
	Transform transform_;

	// return true if any intersections
	virtual bool GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const = 0;
	friend class ConstantMedium;
};

class ShapeObject : public IObject
{
public:
	ShapeObject(const Shape* shape, const Transform& transform, const Material* material) :
		shape_(shape), material_(material), transform_(transform), light_(nullptr)
	{
	}

	ShapeObject(const Shape* shape, const Transform& transform, const Material* material, const Light* light) :
		shape_(shape), material_(material), transform_(transform), light_(light)
	{
	}

	const Material* GetMaterial() { return material_; }
	const Light* GetLight() { return light_; }

	// Evaluate the value of the pdf of a ray targeting the object from the given origin point and direction.
//	virtual double EvaluatePdf(const Vector3& origin, const Vector3& direction) = 0;

	// Generate a random ray
//	virtual Vector3 GenerateRandomRayToObject(const Vector3& origin) = 0;

	bool Intersect(const Ray& r, Intersection& intersection, double tMin, double tMax) const;
	void GetPoints(const Ray& r, std::vector<Intersection>& points, double tMin, double tMax) const;

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
	virtual bool Contains(const Vector3& point)
	{
		return false;
	}
protected:
	const Shape* shape_;
	const Material* material_;
	const Light* light_;
	Transform transform_;
};