#pragma once
#include "../Color.h"
#include "../Intersection.h"
#include "../pdf.h"
#include "../Ray.h"
#include "../Texture.h"

enum class ScatterType
{
	None, // absorption.  Used for Russian Roulette
	Diffuse, // diffuse reflection, generate ray from sampling pdf
	Specular, // specular reflection, use returned specular ray
	Transmission // specular transmission, use returned specular ray
};

struct ScatterResult
{
	ScatterResult();
	Color Attenuation;
	double resultProbability; // probability of this scatter type.  
	Ray SpecularRay; // only set when ScatterType == Specular || ScatterType == Transmission
	std::unique_ptr<Pdf> pdf; // only set when ScatterType == Diffuse
};

class Material
{
private:
public:
	Material();
	virtual bool HasDiffuseComponent() const = 0;
	virtual bool HasSpecularComponent() const = 0;
	virtual bool HasTransmissionComponent() const = 0;

	// Scatter using specified model.  Should be used after Russian Roulette sampling.  Returns false if the material absorbed the light(e.g total internal reflection).
	// If a non-random scatter type is specified, the material should adjust attenuation by the probability light scattering according to that type.
	virtual bool Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& scatterResult) const = 0;

	// Perform Russian Roulette sampling on this material at the intersection point.  Set includeAbsorption to false to force a scatter type.
	virtual ScatterType ScatterRussianRoulette(const Intersection& intersection, bool includeAbsorption) const = 0;

	virtual Color Emit(const Intersection& intersection) const;

	virtual Color BrdfValue(const Intersection& intersection, const Vector3& wi) const = 0;
};

class DiffuseLightMaterial : public Material
{
private:
	Color emittance_;
public:
	DiffuseLightMaterial(const Color& emittance) : emittance_(emittance) {}

	virtual bool HasDiffuseComponent() const;
	virtual bool HasSpecularComponent() const;
	virtual bool HasTransmissionComponent() const;

	// Scatter using specified model. Returns false if the material absorbed the light(e.g total internal reflection).
	virtual bool Scatter(const Intersection& intersection, ScatterType scatterType, ScatterResult& scatterResult) const;

	// Perform Russian Roulette sampling on this material at the intersection point.
	virtual ScatterType ScatterRussianRoulette(const Intersection& intersection, bool includeAbsorption) const;

	virtual Color Emit(const Intersection& intersection) const;

	virtual Color BrdfValue(const Intersection& intersection, const Vector3& wi) const;
};