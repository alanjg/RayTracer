#include "pch.h"
#include "Photon.h"

Photon::Photon()
	:position(), direction(), plane(0), power() /*, theta(0), phi(0), */
{
}

bool Photon::operator<(Photon& rhs) const
{
	return position < rhs.position;
}

bool Photon::operator==(Photon& rhs) const
{
	return position.equals(rhs.position, 1e-6) &&
		power == rhs.power &&
		direction.equals(rhs.direction, 1e-6);
		//phi == rhs.phi &&
		//theta == rhs.theta;
	// don't compare plane for equality
}

bool Photon::operator!=(Photon& rhs) const
{
	return !(*this == rhs);
}

void Photon::ScalePower(Color reflectance)
{
	double pAvg = reflectance.AveragePower();

	power.r *= reflectance.r / pAvg;
	power.g *= reflectance.g / pAvg;
	power.b *= reflectance.b / pAvg;
}
/*
Vector3 Photon::IncomingDirection() const
{
	double sineTheta = sin(theta);
	double cosineTheta = cos(theta);
	double sinePhi = sin(phi);
	double cosinePhi = cos(phi);
	Vector3 photonIncomingDir(sineTheta * cosinePhi, sineTheta * sinePhi, cosineTheta);
	return photonIncomingDir;
}

void Photon::SetIncomingDirection(const Vector3& direction)
{
	phi = atan2(direction[1], direction[0]);
	theta = acos(direction[0]);

}*/