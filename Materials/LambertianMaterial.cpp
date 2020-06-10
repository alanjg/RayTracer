#include "pch.h"
#include "LambertianMaterial.h"


LambertianMaterial::LambertianMaterial(const Color& albedo) :
	albedo_(new ConstantTexture(albedo))
{
}

LambertianMaterial::LambertianMaterial(Texture* albedo) :
	albedo_(albedo)
{
}

bool LambertianMaterial::Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& result) const
{
	result.pdf = std::unique_ptr<Pdf>(new CosinePdf(intersection.normal));
	result.Attenuation = albedo_->Value(intersection.u, intersection.v, intersection.point);
	result.resultProbability = 1.0;
	return true;
}

bool LambertianMaterial::HasDiffuseComponent() const
{
	return true;
}

bool LambertianMaterial::HasSpecularComponent() const
{
	return false;
}

bool LambertianMaterial::HasTransmissionComponent() const
{
	return false;
}

// Perform Russian Roulette sampling on this material at the intersection point.
ScatterType LambertianMaterial::ScatterRussianRoulette(const Intersection& intersection, bool includeAbsorption) const
{
	Color attenuation = albedo_->Value(intersection.u, intersection.v, intersection.point);
	if (!includeAbsorption || Random::NextReal() < attenuation.AveragePower())
	{
		return ScatterType::Diffuse;
	}
	else
	{
		return ScatterType::None;
	}
}

Color LambertianMaterial::BrdfValue(const Intersection& intersection, const Vector3& wi) const
{
	Color attenuation = albedo_->Value(intersection.u, intersection.v, intersection.point);
	return attenuation * M_1_PI;
}
