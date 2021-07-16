#pragma once
#include "Object.h"
#include "Intersection.h"
#include "Ray.h"

class IntersectionAccelerator
{
public:
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const = 0;

	// Some surfaces may be overlapping.  The tracer must correctly resolve these into a single intersection.
	//virtual bool GetClosestIntersections(const Ray& ray, std::vector<Intersection>& intersections, double tMin, double tMax) const = 0;
}; 
