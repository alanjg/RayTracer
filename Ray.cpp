#include "pch.h"
#include "Math.h"
#include "Ray.h"

Ray::Ray()
	: origin(), direction()
{

}

bool RayBoxIntersect(const Ray& ray, const Vector3& vmin, const Vector3& vmax)
{
	int in = 0;
	double t;
	bool skip = false;
	if (ray.origin[0] < vmin[0])
	{
		if (ray.direction[0] <= 0) //wrong way
			return false;

		t = (vmin[0] - ray.origin[0]) / ray.direction[0];

	}
	else if (ray.origin[0] > vmax[0])
	{
		if (ray.direction[0] >= 0) //wrong way
			return false;

		t = (vmax[0] - ray.origin[0]) / ray.direction[0];

	}
	else
	{
		in++;
		skip = true;
	}
	if (!skip)
	{
		double p1 = ray.origin[1] + t * ray.direction[1];
		double p2 = ray.origin[2] + t * ray.direction[2];

		if (p1 >= vmin[1] && p1 <= vmax[1] && p2 >= vmin[2] && p2 <= vmax[2])
			return true;
	}
	skip = false;
	if (ray.origin[1] < vmin[1])
	{
		if (ray.direction[1] <= 0) //wrong way
			return false;

		t = (vmin[1] - ray.origin[1]) / ray.direction[1];

	}
	else if (ray.origin[1] > vmax[1])
	{
		if (ray.direction[1] >= 1) //wrong way
			return false;

		t = (vmax[1] - ray.origin[1]) / ray.direction[1];

	}
	else
	{
		in++;
		skip = true;
	}
	if (!skip)
	{
		double p1 = ray.origin[0] + t * ray.direction[0];
		double p2 = ray.origin[2] + t * ray.direction[2];

		if (p1 >= vmin[0] && p1 <= vmax[0] && p2 >= vmin[2] && p2 <= vmax[2])
			return true;
	}
	skip = false;
	if (ray.origin[2] < vmin[2])
	{
		if (ray.direction[2] <= 0) //wrong way
			return false;

		t = (vmin[2] - ray.origin[2]) / ray.direction[2];

	}
	else if (ray.origin[2] > vmax[2])
	{
		if (ray.direction[2] >= 1) //wrong way
			return false;

		t = (vmax[2] - ray.origin[2]) / ray.direction[2];

	}
	else
	{
		in++;
		skip = true;
	}
	if (!skip)
	{
		double p1 = ray.origin[0] + t * ray.direction[0];
		double p2 = ray.origin[1] + t * ray.direction[1];

		if (p1 >= vmin[0] && p1 <= vmax[0] && p2 >= vmin[1] && p2 <= vmax[1])
			return true;
	}
	return in == 3;
}