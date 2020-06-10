#pragma once
#include "Object.h"
#include "Intersection.h"
#include "Ray.h"

class IntersectionAccelerator
{
public:
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const = 0;
}; 
