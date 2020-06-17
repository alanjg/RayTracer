#pragma once
#include "Color.h"

#include "Ray.h"
#include "Vector.h"

class IObject;
class Object;
//struct ShapeIntersection
//{
//	double intersectionTime;
//	Vector3 point;
//
//	// normal points towards the direction of the incident ray.  For internalIntersections it points inwards.
//	Vector3 normal;
//
//	// true if the ray hit the inside of the object
//	bool internalIntersection;
//
//	// texture coordinates
//	double u, v;
//};

struct Intersection
{
	Ray incidentRay;
	double distance;
	Vector3 point;

	// normal points towards the direction of the incident ray.  For internalIntersections it points inwards.
	Vector3 normal;

	// true if the ray hit the inside of the object
	bool internalIntersection; 

	// texture coordinates
	double u, v; 

	// set when the ray originates inside an object(from ray transmission), nullptr otherwise
	IObject* sourceObject;

	// the object his by the ray, nullptr if no intersection is detected
	IObject* hitObject;

	// compares by distance
	bool operator<(const Intersection& rhs) const;

	Intersection();
};

//struct HitRecord
//{
//	Intersection intersection;
//	Ray incidentRay;
//	Object* sourceObject; // set when the ray originates inside an object(from ray transmission), nullptr otherwise
//	Object* hitObject; // the object his by the ray, nullptr if no intersection is detected
//};

// returns true if the testIntersection is a closer hit.  Will also handle the case of a transmission ray leaving one object and hitting another by returning the other object.
bool IsCloserIntersection(const Intersection& currentIntersection, const Intersection& testIntersection);