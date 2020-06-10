#include "pch.h"
#include "IsotropicMaterial.h"

bool IsotropicMaterial::Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& result) const
{
	result.pdf.reset(new SphericalNormalPdf());
	result.Attenuation = albedo_->Value(intersection.u, intersection.v, intersection.point);
	result.resultProbability = 1.0;
	return true;
}

bool IsotropicMaterial::HasDiffuseComponent() const
{
	return true;
}

bool IsotropicMaterial::HasSpecularComponent() const
{
	return false;
}

bool IsotropicMaterial::HasTransmissionComponent() const
{
	return false;
}

ScatterType IsotropicMaterial::ScatterRussianRoulette(const Intersection& intersection, bool includeAbsorption) const
{
	return ScatterType::Diffuse;
}

Color IsotropicMaterial::BrdfValue(const Intersection& intersection, const Vector3& wi) const
{
	return Color(0, 0, 0);
}
