#include "pch.h"
#include "Light.h"
#include "Materials/Material.h"
#include "Matrix.h"
#include "Options.h"
#include "Objects/Polygon.h"

Light::Light(const Color& power) :
	power_(power)
{
}


PointLight::PointLight(const Vector3& position, const Color& power, const Color& emittance) :
	Light(power), position_(position), emittance_(emittance)
{
}

Photon PointLight::GenerateRandomPhoton(Vector3& direction) const
{
	GenerateRandomDirectionInSphere(direction);
	Photon photon;
	photon.position = position_;
	photon.power = power_;
	return photon;
}

Color PointLight::SampleLightEmittance(const Intersection& intersection, Vector3& sampleDirection, double& samplePdf) const
{
	sampleDirection = position_ - intersection.point;
	sampleDirection.Normalize();
	samplePdf = 1.0;
	return emittance_;
}

double PointLight::EvaluateRadianceTransfer(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const
{
	double surfaceCosine = std::abs(surfaceIntersection.normal.Dot(shadowRayIntersection.incidentRay.direction));
	double lightCosine = 1.0;

	double distSquared = (surfaceIntersection.point - position_).MagnitudeSquared();
	double g = surfaceCosine * lightCosine / distSquared;
	double v = IsOccluded(surfaceIntersection, shadowRayIntersection) ? 0.0 : 1.0;
	return g * v;
}

bool PointLight::IsOccluded(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const
{
	if (shadowRayIntersection.hitObject == nullptr)
	{
		return false;
	}
	else
	{
		double distToLight = (position_ - surfaceIntersection.point).Magnitude();
		if (GreaterThan(distToLight, shadowRayIntersection.distance))
		{
			// We hit something closer than the light point
			return true;
		}
		else
		{
			return false;
		}
	}
}


SpotLight::SpotLight(const Vector3& position, const Vector3& direction, float totalAngle, float falloffAngle, const Color& power, const Color& emittance) :
	Light(power), position_(position), direction_(direction), totalAngle_(totalAngle), falloffAngle_(falloffAngle), emittance_(emittance)
{
}

Photon SpotLight::GenerateRandomPhoton(Vector3& direction) const
{
	GenerateRandomDirectionInSphere(direction);
	Photon photon;
	photon.position = position_;
	photon.power = power_;
	return photon;
}

Color SpotLight::SampleLightEmittance(const Intersection& intersection, Vector3& sampleDirection, double& samplePdf) const
{
	sampleDirection = position_ - intersection.point;
	sampleDirection.Normalize();
	samplePdf = 1.0;
	return emittance_;
}

double SpotLight::EvaluateRadianceTransfer(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const
{
	double surfaceCosine = std::abs(surfaceIntersection.normal.Dot(shadowRayIntersection.incidentRay.direction));
	double lightCosine = 1.0;

	double distSquared = (surfaceIntersection.point - position_).MagnitudeSquared();
	double g = surfaceCosine * lightCosine / distSquared;
	double v = IsOccluded(surfaceIntersection, shadowRayIntersection) ? 0.0 : 1.0;
	return g * v;
}

bool SpotLight::IsOccluded(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const
{
	if (shadowRayIntersection.hitObject == nullptr)
	{
		return false;
	}
	else
	{
		double distToLight = (position_ - surfaceIntersection.point).Magnitude();
		if (GreaterThan(distToLight, shadowRayIntersection.distance))
		{
			// We hit something closer than the light point
			return true;
		}
		else
		{
			return false;
		}
	}
}

/*
Vector3 PointLight::GetRandomPointOnLight() const
{
	return position_;
}

std::unique_ptr<Pdf> PointLight::GetLightPdf() const
{
	// todo
	return std::unique_ptr<Pdf>(new CosinePdf(Vector3()));
}


AreaLight::AreaLight(const Color& power) :
	Light(power)
{

}


Vector3 RectangleAreaLight::GetRandomPointOnLight() const
{
	return GetRandomPointInRect();
}

std::unique_ptr<Pdf> RectangleAreaLight::GetLightPdf() const
{
	return std::unique_ptr<Pdf>(new RectanglePdf(rmin_, rmin_ + rdu_, rmin_ + rdv_));
}
*/


AreaLight::AreaLight(const Color& power, IObject* object)
	:Light(power), lightObject_(object)
{
}

IObject* AreaLight::GetLightObject() const
{
	return lightObject_;
}

std::pair<std::unique_ptr<RectangleAreaLight>, std::unique_ptr<IObject>> CreateRectangleAreaLight(const Vector3& ll, const Vector3& lr, const Vector3& ul, const Color& power, const Color& emittance, const Transform& transform, const Material* material)
{
	std::vector<Vector3> points;
	points.push_back(ll);
	points.push_back(lr);
	points.push_back(lr + ul - ll);
	points.push_back(ul);
	
	Polygon* poly = new Polygon(points);
	ShapeObject* rectangle = new ShapeObject(poly, transform, material);
	RectangleAreaLight* light = new RectangleAreaLight(rectangle, ll, lr, ul, power, emittance);
	return std::make_pair(std::unique_ptr<RectangleAreaLight>(light), std::unique_ptr<IObject>(rectangle));
}

RectangleAreaLight::RectangleAreaLight(IObject* lightObject, const Vector3& ll, const Vector3& lr, const Vector3& ul, const Color& power, const Color& emittance) :
	AreaLight(power, lightObject), rmin_(ll), rdu_(lr - ll), rdv_(ul - ll), emittance_(emittance)
{
	normal_ = rdu_.Cross(rdv_);
	normal_.Normalize();
	center_ = ll + (lr - ll) / 2 + (ul - ll) / 2;
}

Vector3 RectangleAreaLight::GetRandomPointInRect() const
{
	// pick a random point in the rect
	double du = (rand() / (double)RAND_MAX);
	double dv = (rand() / (double)RAND_MAX);
	return rmin_ + rdu_ * du + rdv_ * dv;
}

Photon RectangleAreaLight::GenerateRandomPhoton(Vector3& direction) const
{
	Vector3 position = GetRandomPointInRect();
	GenerateRandomDirectionInHemisphereCosineWeighted(normal_, rdu_, direction);

	Photon photon;
	photon.position = position;
	photon.power = power_;
	return photon;
}

Color RectangleAreaLight::SampleLightEmittance(const Intersection& intersection, Vector3& sampleDirection, double& samplePdf) const
{
	Vector3 position = GetRandomPointInRect();
	sampleDirection = position - intersection.point;
	sampleDirection.Normalize();
	
	double lightArea = rdu_.Cross(rdv_).Magnitude();
	samplePdf = 1.0 / (lightArea);
	return emittance_;
}

double RectangleAreaLight::EvaluateRadianceTransfer(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const
{
	double surfaceCosine = std::abs(surfaceIntersection.normal.Dot(shadowRayIntersection.incidentRay.direction));
	double lightCosine = std::abs(shadowRayIntersection.incidentRay.direction.Dot(normal_));
	
	double distSquared = (shadowRayIntersection.point - surfaceIntersection.point).MagnitudeSquared();
	double g = surfaceCosine * lightCosine / distSquared;
	double v = IsOccluded(surfaceIntersection, shadowRayIntersection) ? 0.0 : 1.0;
	return g * v;
}

bool RectangleAreaLight::IsOccluded(const Intersection& surfaceIntersection, const Intersection& shadowRayIntersection) const
{
	return shadowRayIntersection.hitObject != GetLightObject() || shadowRayIntersection.normal.Dot(normal_) < 0 || shadowRayIntersection.internalIntersection;
}