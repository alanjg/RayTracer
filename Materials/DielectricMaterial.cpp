#include "pch.h"
#include "DielectricMaterial.h"

DielectricMaterial::DielectricMaterial(double ri) :
	ri_(ri)
{
}

bool DielectricMaterial::Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& result) const
{
	result.Attenuation = Color(1, 1, 1);
	double niOverNt;
	
	Vector3 outwardNormal = intersection.normal;
	double cosine = -intersection.incidentRay.direction.Dot(intersection.normal);
	if (intersection.internalIntersection)
	{
		niOverNt = ri_;
		cosine *= ri_;
	}
	else
	{
		niOverNt = 1 / ri_;
	}

	Ray scattered;
	scattered.origin = intersection.point;
	double reflectionProbability = 1.0;
	if (Refract(intersection.incidentRay.direction, outwardNormal, niOverNt, scattered.direction))
	{
		reflectionProbability = Schlick(cosine, niOverNt);
	}
	else
	{
		if (scatterType == ScatterType::Transmission)
		{
			return false;
		}
	}
	
	if (scatterType == ScatterType::Specular)
	{
		result.resultProbability = reflectionProbability;
		scattered.direction = intersection.incidentRay.direction.Reflect(intersection.normal);
	}
	else
	{
		result.resultProbability = 1 - reflectionProbability;
	}
	
	result.SpecularRay = scattered;
	return true;
}

bool DielectricMaterial::HasDiffuseComponent() const
{
	return false;
}

bool DielectricMaterial::HasSpecularComponent() const
{
	return true;
}

bool DielectricMaterial::HasTransmissionComponent() const
{
	return true;
}

ScatterType DielectricMaterial::ScatterRussianRoulette(const Intersection& intersection, bool includeAbsorption) const
{
	// todo fix
	Vector3 outwardNormal;
	Vector3 reflected = intersection.incidentRay.direction.Reflect(intersection.normal);
	double niOverNt;
	
	Ray scattered;
	Vector3 refracted;
	double reflectionProbability;
	double cosine = -intersection.incidentRay.direction.Dot(intersection.normal);
	outwardNormal = intersection.normal;
	if (intersection.internalIntersection)
	{
		niOverNt = ri_;
		cosine *= ri_;
	}
	else
	{
		niOverNt = 1 / ri_;
	}
	scattered.origin = intersection.point;
	if (Refract(intersection.incidentRay.direction, outwardNormal, niOverNt, refracted))
	{
		reflectionProbability = Schlick(cosine, niOverNt);
	}
	else
	{
		reflectionProbability = 1.0;
	}

	if (Random::NextReal() < reflectionProbability)
	{
		return ScatterType::Specular;
	}
	else
	{
		return ScatterType::Transmission;
	}
}

Color DielectricMaterial::BrdfValue(const Intersection& intersection, const Vector3& wi) const
{
	return Color(0, 0, 0);
}
