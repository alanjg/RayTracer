#pragma once
#include "Material.h"
#include "../Color.h"
#include "../Intersection.h"
#include "../pdf.h"
#include "../Ray.h"
#include "../Texture.h"

class LambertianMaterial : public Material
{
private:
	Texture* albedo_;
public:
	LambertianMaterial(Texture* albedo);
	LambertianMaterial(const Color& albedo);

	virtual bool HasDiffuseComponent() const;
	virtual bool HasSpecularComponent() const;
	virtual bool HasTransmissionComponent() const;

	// Scatter using specified model. Returns false if the material absorbed the light(e.g total internal reflection).
	virtual bool Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& scatterResult) const;

	// Perform Russian Roulette sampling on this material at the intersection point.
	virtual ScatterType ScatterRussianRoulette(const Intersection& intersection, bool includeAbsorption) const;

	virtual Color BrdfValue(const Intersection& intersection, const Vector3& wi) const;
};
