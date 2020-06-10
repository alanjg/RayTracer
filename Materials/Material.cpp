#include "pch.h"
#include "Material.h"


Material::Material()
{
}

ScatterResult::ScatterResult()
	:Attenuation(0,0,0), SpecularRay(), pdf(nullptr)
{
}

Color Material::Emit(const Intersection& intersection) const
{
	return Color(0, 0, 0);
}

bool DiffuseLightMaterial::HasDiffuseComponent() const
{
	return false;
}

bool DiffuseLightMaterial::HasSpecularComponent() const
{
	return false;
}

bool DiffuseLightMaterial::HasTransmissionComponent() const
{
	return false;
}

bool DiffuseLightMaterial::Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& scatterResult) const
{
	return false;
}

ScatterType DiffuseLightMaterial::ScatterRussianRoulette(const Intersection& intersection, bool includeAbsoprtion) const
{
	return ScatterType::None;
}

Color DiffuseLightMaterial::Emit(const Intersection& intersection) const
{
	return emittance_;
}

Color DiffuseLightMaterial::BrdfValue(const Intersection& intersection, const Vector3& wi) const
{
	return Color(0,0,0);
}