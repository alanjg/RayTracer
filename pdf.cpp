#include "pch.h"
#include "pdf.h"

CosinePdf::CosinePdf(const Vector3& basisDirection)
{
	basis_.BuildFromVector(basisDirection);
}

double CosinePdf::Value(const Vector3& direction) const
{
	double cosine = direction.Dot(basis_[2]);
	if (cosine > 0)
	{
		return cosine / M_PI;
	}
	else
	{
		return 0;
	}
}

Vector3 CosinePdf::GenerateRandom() const
{
	return basis_.ToLocal(RandomCosineDirection());
}

RectanglePdf::RectanglePdf(const Vector3& ll, const Vector3& lr, const Vector3& ul)
	:rmin_(ll), rdu_(lr-ll), rdv_(ul)
{
}

double RectanglePdf::Value(const Vector3& direction) const
{
	double area = rdu_.Cross(rdv_).Magnitude();
	return 1.0 / area;
}

Vector3 RectanglePdf::GenerateRandom() const
{
	// pick a random point in the rect
	double du = (rand() / (double)RAND_MAX);
	double dv = (rand() / (double)RAND_MAX);
	return rmin_ + rdu_ * du + rdv_ * dv;
}

double SphericalNormalPdf::Value(const Vector3& direction) const
{
	double surfaceArea = 4 * M_PI;
	return 1.0 / surfaceArea;
}

Vector3 SphericalNormalPdf::GenerateRandom() const
{
	Vector3 dir = RandomInUnitSphere();
	dir.Normalize();
	return dir;
}