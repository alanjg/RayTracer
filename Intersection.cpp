#include "pch.h"
#include "Intersection.h"

bool Intersection::operator<(const Intersection& rhs) const
{
	return distance < rhs.distance;
}

Intersection::Intersection()
{
	internalIntersection = false;
	sourceObject = nullptr;
	hitObject = nullptr;
	u = 0;
	v = 0;
	distance = std::numeric_limits<double>::max();
}

bool IsCloserIntersection(const Intersection& currentIntersection, const Intersection& testIntersection)
{
	// Case 1 - no source object, so 
	if (currentIntersection.sourceObject == nullptr && testIntersection.sourceObject == nullptr)
	{
		if (testIntersection.distance < currentIntersection.distance)
		{

		}
	}
	return false;
}