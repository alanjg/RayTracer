#include "pch.h"
#include "OrthoNormalBasis.h"

OrthoNormalBasis::OrthoNormalBasis()
{
}

Vector3& OrthoNormalBasis::operator[](int i)
{
	return basisVectors_[i];
}

const Vector3& OrthoNormalBasis::operator[](int i) const
{
	return basisVectors_[i];
}

Vector3 OrthoNormalBasis::ToLocal(const Vector3& v) const
{
	return v[0] * basisVectors_[0] + v[1] * basisVectors_[1] + v[2] * basisVectors_[2];
}

void OrthoNormalBasis::BuildFromVector(const Vector3& v)
{
	basisVectors_[2] = v;
	basisVectors_[2].Normalize();

	// pick a vector that is not parallel to v
	Vector3 nonParallelVector;
	if (std::abs(basisVectors_[2][0]) > 0.9)
	{
		nonParallelVector.Set(0, 1, 0);
	}
	else
	{
		nonParallelVector.Set(1, 0, 0);
	}

	basisVectors_[1] = basisVectors_[2].Cross(nonParallelVector);
	basisVectors_[1].Normalize();

	basisVectors_[0] = basisVectors_[2].Cross(basisVectors_[1]); // already normalized
}