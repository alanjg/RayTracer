#pragma once
#include "OrthoNormalBasis.h"
#include "Vector.h"

// Probability density function
class Pdf
{
public:
	virtual double Value(const Vector3& direction) const = 0;
	virtual Vector3 GenerateRandom() const = 0;
};

class CosinePdf : public Pdf
{
private:
	OrthoNormalBasis basis_;
public:
	CosinePdf(const Vector3& basisDirection);
	virtual double Value(const Vector3& direction) const;
	virtual Vector3 GenerateRandom() const;
};

class RectanglePdf : public Pdf
{
private:
	Vector3 rmin_, rdu_, rdv_;
public:
	RectanglePdf(const Vector3& ll, const Vector3& lr, const Vector3& ul);
	virtual double Value(const Vector3& direction) const;
	virtual Vector3 GenerateRandom() const;
};

class SphericalNormalPdf : public Pdf
{
public:
	virtual double Value(const Vector3& direction) const;
	virtual Vector3 GenerateRandom() const;
};