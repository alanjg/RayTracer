#pragma once
#include "Vector.h"

class OrthoNormalBasis
{
private:
	Vector3 basisVectors_[3];
public:
	OrthoNormalBasis();
	Vector3& operator[](int i);
	const Vector3& operator[](int i) const;
	Vector3 ToLocal(const Vector3& v) const;
	void BuildFromVector(const Vector3& v);
};

