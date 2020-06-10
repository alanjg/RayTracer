#pragma once
#include "Vector.h"

struct Ray
{
	Vector3 origin;
	Vector3 direction;
	Ray();
};


bool RayBoxIntersect(const Ray& ray, const Vector3& vmin, const Vector3& vmax);