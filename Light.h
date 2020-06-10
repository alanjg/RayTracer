#pragma once
#include "Color.h"
#include "pdf.h"
#include "Photon.h"
#include "Intersection.h"
#include "Materials/Material.h"
#include "Ray.h"
#include "Transform.h"
#include "Vector.h"

class Light
{
protected:
	Light(const Color& power);

	Color power_;
public:	
	Color Power() const { return power_; }

	virtual Photon GenerateRandomPhoton(Vector3& direction) const = 0;

	// sampleDirection is given by the light and should be used for visibility checks.  
	// samplePdf is the pdf for sampling the light.  
	// gValue is the evaluated value of G(shadowRay.origin, lightIntersection.point) = cos(originNormal, shadowRay.direction) * cos (lightNormal, -shadowRay.direction) / dist^2
	virtual Color SampleLightEmittance(const Intersection& intersection, Vector3& sampleDirection, double& samplePdf) const = 0;

	// radianceTransfer = G(intersection.point, shadowIntersection.point) * Visibility(light)
	// Visibility = 0 if occluded, 1 if visible.
	// G(shadowRay.origin, lightIntersection.point) = cos(intersection.normal, shadowRay.direction) * cos(shadowIntersection.normal, -shadowRay.direction) / dist^2
	virtual double EvaluateRadianceTransfer(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const = 0;

	virtual bool IsOccluded(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const = 0;
	virtual ~Light() { };
};

class PointLight : public Light
{
private:
	Vector3 position_;
	Color emittance_;
public:
	PointLight(const Vector3& position, const Color& power, const Color& emittance);
	virtual Photon GenerateRandomPhoton(Vector3& direction) const;
	virtual Color SampleLightEmittance(const Intersection& intersection, Vector3& sampleDirection, double& samplePdf) const;
	virtual double EvaluateRadianceTransfer(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const;
	virtual bool IsOccluded(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const;
	virtual ~PointLight() {}
};

class AreaLight : public Light
{
	Object* lightObject_;
protected:
	AreaLight(const Color& power, Object* object);

public:
	Object* GetLightObject() const;
	virtual ~AreaLight() {}
};

class RectangleAreaLight : public AreaLight
{
private:
	Color emittance_;

	Vector3 rmin_, rdu_, rdv_, center_;
	Vector3 normal_;
	Vector3 GetRandomPointInRect() const;
public:
	RectangleAreaLight(Object* lightObject, const Vector3& ll, const Vector3& lr, const Vector3& ul, const Color& power, const Color& emittance);
	virtual Photon GenerateRandomPhoton(Vector3& direction) const;
	virtual Color SampleLightEmittance(const Intersection& intersection, Vector3& sampleDirection, double& samplePdf) const;
	virtual double EvaluateRadianceTransfer(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const;
	virtual bool IsOccluded(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const;
	virtual ~RectangleAreaLight() {}
};

std::pair<std::unique_ptr<RectangleAreaLight>, std::unique_ptr<Object>> CreateRectangleAreaLight(const Vector3& ll, const Vector3& lr, const Vector3& ul, const Color& power, const Color& emittance, const Transform& transform, const Material* material);

/*

class DirectionalLight : public Light
{

};

class AreaLight : public Light
{
protected:
	AreaLight(const Color& power);
};

class RectangleAreaLight : public AreaLight
{
protected:
	Vector3 rmin_, rdu_, rdv_, center_;
	Vector3 normal_;

	Vector3 GetRandomPointInRect() const;
public:
	RectangleAreaLight(const Vector3& ll, const Vector3& lr, const Vector3& ul, const Color& power);
	virtual Photon GenerateRandomPhoton(Vector3& direction) const;
	virtual Vector3 GetRandomPointOnLight() const;

	virtual std::unique_ptr<Pdf> GetLightPdf() const;
};
*/