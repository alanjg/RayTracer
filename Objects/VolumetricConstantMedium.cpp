#include "pch.h"
#include "VolumetricConstantMedium.h"
#include "../Materials/Material.h"
#include "../Materials/IsotropicMaterial.h"

VolumetricConstantMedium::VolumetricConstantMedium(Shape* boundary, double density, const Transform& transform, Texture* albedo)
	: Object(transform, new IsotropicMaterial(albedo)), boundary_(boundary), density_(density)
{
}

bool VolumetricConstantMedium::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	if (firstPointOnly)
	{
		Ray localRay = transform_.TransformWorldToLocal(ray);
		Intersection i1, i2;
		if (boundary_->Intersect(localRay, i1, std::numeric_limits<double>::min(), std::numeric_limits<double>::max()))
		{
			if (boundary_->Intersect(localRay, i2, i1.distance + INTERSECTION_ADJUSTMENT, std::numeric_limits<double>::max()))
			{
				i1.distance = std::max(tMin, i1.distance);
				i2.distance = std::min(tMax, i2.distance);
				if (i1.distance >= i2.distance)
				{
					return false;
				}
				i1.distance = std::max(i1.distance, 0.0);
				double dist = i2.distance - i1.distance;
				double hitDist = -(1 / density_) * log(Random::NextReal());
				if (hitDist < dist)
				{
					firstIntersection.distance = i1.distance + hitDist;
					firstIntersection.point = localRay.origin + firstIntersection.distance * localRay.direction;
					firstIntersection.normal = RandomInUnitSphere();
					firstIntersection = transform_.TransformLocalIntersectionToWorld(ray, firstIntersection);
					firstIntersection.u = 0;
					firstIntersection.v = 0;
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

VolumetricConstantMedium::~VolumetricConstantMedium()
{
}

Vector3 VolumetricConstantMedium::GetMin() const
{
	return transform_.TransformLocalPointToWorld(boundary_->GetMin());
}

Vector3 VolumetricConstantMedium::GetMax() const
{
	return transform_.TransformLocalPointToWorld(boundary_->GetMax());
}