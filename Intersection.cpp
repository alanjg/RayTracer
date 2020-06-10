#include "pch.h"
#include "Intersection.h"

bool Intersection::operator<(const Intersection& rhs) const
{
	return intersectionTime < rhs.intersectionTime;
}

Intersection::Intersection()
{
	internalIntersection = false;
	sourceObject = nullptr;
	hitObject = nullptr;
	u = 0;
	v = 0;
	intersectionTime = std::numeric_limits<double>::max();
}

bool IsCloserIntersection(const Intersection& currentIntersection, const Intersection& testIntersection)
{
	// Case 1 - no source object, so 
	if (currentIntersection.sourceObject == nullptr && testIntersection.sourceObject == nullptr)
	{
		if (testIntersection.intersectionTime < currentIntersection.intersectionTime)
		{

		}
	}
	return false;
}