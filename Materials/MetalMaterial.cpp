#include "pch.h"
#include "MetalMaterial.h"

MetalMaterial::MetalMaterial(const Color& albedo, double fuzziness) :
	albedo_(albedo), fuzziness_(fuzziness)
{
}

bool MetalMaterial::HasDiffuseComponent() const
{
	return false;
}

bool MetalMaterial::HasSpecularComponent() const
{
	return true;
}

bool MetalMaterial::HasTransmissionComponent() const
{
	return false;
}

bool MetalMaterial::Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& scatterResult) const
{
	Vector3 reflected = intersection.incidentRay.direction.Reflect(intersection.normal);
	Ray scattered;
	scattered.origin = intersection.point;
	scattered.direction = reflected + fuzziness_ * RandomInUnitSphere();
	scattered.direction.Normalize();
	scatterResult.SpecularRay = scattered;
	scatterResult.resultProbability = 1.0;
	scatterResult.Attenuation = albedo_;
	return scattered.direction.Dot(intersection.normal) > 0;
}

// Perform Russian Roulette sampling on this material at the intersection point.
ScatterType MetalMaterial::ScatterRussianRoulette(const Intersection& intersection, bool includeAbsorption) const
{
	Color attenuation = albedo_;
	if (!includeAbsorption || Random::NextReal() < attenuation.AveragePower())
	{
		return ScatterType::Specular;
	}
	else
	{
		return ScatterType::None;
	}
}

Color MetalMaterial::BrdfValue(const Intersection& intersection, const Vector3& wi) const
{
	return Color(0, 0, 0);
}