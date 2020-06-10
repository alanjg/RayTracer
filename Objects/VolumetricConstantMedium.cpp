#include "pch.h"
#include "VolumetricConstantMedium.h"
#include "../Materials/Material.h"
#include "../Materials/IsotropicMaterial.h"

VolumetricConstantMedium::VolumetricConstantMedium(Object* boundary, double density, const Transform& transform, Texture* albedo)
	:Object(transform, new IsotropicMaterial(albedo)), boundary_(boundary), density_(density)
{
}

bool VolumetricConstantMedium::GetIntersectionPoints(const Ray& r, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	if (firstPointOnly)
	{
		Intersection i1, i2;
		if (boundary_->Intersect(r, i1, std::numeric_limits<double>::min(), std::numeric_limits<double>::max()))
		{
			static int total = 0;
			static int hits = 0;
			total++;
			if (boundary_->Intersect(r, i2, i1.intersectionTime + INTERSECTION_ADJUSTMENT, std::numeric_limits<double>::max()))
			{
				i1.intersectionTime = std::max(tMin, i1.intersectionTime);
				i2.intersectionTime = std::min(tMax, i2.intersectionTime);
				if (i1.intersectionTime >= i2.intersectionTime)
				{
					return false;
				}
				i1.intersectionTime = std::max(i1.intersectionTime, 0.0);
				double dist = i2.intersectionTime - i1.intersectionTime;
				double hitDist = -(1 / density_) * log(Random::NextReal());
				if (hitDist < dist)
				{
					firstIntersection.intersectionTime = i1.intersectionTime + hitDist;
					firstIntersection.point = r.origin + firstIntersection.intersectionTime * r.direction;
					firstIntersection.normal = RandomInUnitSphere();
					firstIntersection.u = 0;
					firstIntersection.v = 0;
					hits++;
					return true;
				}
			}
		}
		return false;
	}
	else
	{
		// csg should be an error
		return false;
	}
}

bool VolumetricConstantMedium::Contains(const Vector3& point)
{
	return boundary_->Contains(point);
}

VolumetricConstantMedium::~VolumetricConstantMedium()
{
}

Vector3 VolumetricConstantMedium::GetMin() const
{
	return boundary_->GetMin();
}

Vector3 VolumetricConstantMedium::GetMax() const
{
	return boundary_->GetMax();
}