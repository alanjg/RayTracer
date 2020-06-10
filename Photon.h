#pragma once

#include "Color.h"
#include "Vector.h"

struct Photon
{
	Vector3 position;
	Color power;
	Vector3 direction;

	int plane;
	bool operator<(Photon& rhs) const;
	bool operator==(Photon& rhs) const;
	bool operator!=(Photon& rhs) const;
	void ScalePower(Color reflectance);

	// todo - memory optimized
	//double phi, theta;
	//Vector3 IncomingDirection() const;
	//void SetIncomingDirection(const Vector3& direction);
	Photon();
};