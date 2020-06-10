#include "pch.h"
#include "Math.h"
#include "Matrix.h"
#include "Vector.h"

std::ostream& operator<<(std::ostream& out, const Vector2& v)
{
	return out << v[0] << " " << v[1];
}

std::istream& operator>>(std::istream& in, Vector2& v)
{
	return in >> v[0] >> v[1];
}

std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << v[0] << " " << v[1] << " " << v[2];
}

std::istream& operator>>(std::istream& in, Vector3& v)
{
	return in >> v[0] >> v[1] >> v[2];
}

std::ostream& operator<<(std::ostream& out, const Vector4& v)
{
	return out << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
}

std::istream& operator>>(std::istream& in, Vector4& v)
{
	return in >> v[0] >> v[1] >> v[2] >> v[3];
}

Vector3 RandomInUnitSphere()
{
	Vector3 result;
	// rejection sampling
	do
	{
		Vector3 randomVector(Random::NextReal(), Random::NextReal(), Random::NextReal());
		result = 2.0 * randomVector - Vector3(1, 1, 1);
	} while (result.MagnitudeSquared() >= 1.0);
	return result;
}

Vector3 RandomInUnitDisk()
{
	Vector3 result;
	do
	{
		result = 2.0 * Vector3(Random::NextReal() - 1, Random::NextReal() - 1, 0);
	} while (result.MagnitudeSquared() >= 1.0);
	return result;
}

Vector3 RandomCosineDirection()
{
	double r1 = Random::NextReal();
	double r2 = Random::NextReal();
	double z = sqrt(1 - r2);
	double phi = 2 * M_PI * r1;
	Vector3 result(cos(phi) * 2 * sqrt(r2), sin(phi) * 2 * sqrt(r2), z);
	result.Normalize();
	return result;
}

bool Refract(const Vector3& vector, const Vector3& normal, double ni_over_nt, Vector3& refracted)
{
	double Ci = -normal.Dot(vector);
	// T = Nit*I + (Nit*Ci - sqrt(1+Nit^2*(Ci^2-1)))*N
	double radical = 1 + ni_over_nt * ni_over_nt * (Ci * Ci - 1);
	if (radical >= 0)
	{
		refracted = ni_over_nt * vector + (ni_over_nt * Ci - sqrt(radical)) * normal;
		refracted.Normalize();
		return true;
	}
	else
	{
		//radical < 0 means total internal reflection, no light transmitted
		return false;
	}
}

void GenerateRandomDirectionInSphere(Vector3& direction)
{
	//randomly pick a direction, and check that it's in the unit sphere
	do
	{
		direction[0] = (rand() / (double)RAND_MAX) * 2 - 1;
		direction[1] = (rand() / (double)RAND_MAX) * 2 - 1;
		direction[2] = (rand() / (double)RAND_MAX) * 2 - 1;
	} while (direction.Magnitude() > 1 || direction.Magnitude() == 0);

	direction.Normalize();
}

void GenerateRandomDirectionInHemisphereCosineWeighted(const Vector3& normal, const Vector3& surface, Vector3& direction)
{
	double eta1 = rand() / (double)RAND_MAX;
	double eta2 = rand() / (double)RAND_MAX;
	double theta = acos(sqrt(eta1));
	double phi = 2 * M_PI * eta2;

	Matrix44 rotation1, rotation2;
	rotation1.MakeRotateRad(theta, surface);
	rotation2.MakeRotateRad(phi, normal);

	direction = rotation2.GetTranspose() * (rotation1.GetTranspose() * normal);
	direction.Normalize();
}

bool contains(const Vector3& vmin, const Vector3& vmax, const Vector3& point, double tolerance)
{
	return LessThan(vmin[0], point[0], tolerance) && LessThan(point[0], vmax[0]) &&
		LessThan(vmin[1], point[1]) && LessThan(point[1], vmax[1]) &&
		LessThan(vmin[2], point[2]) && LessThan(point[2], vmax[2]);
}

bool contains(const Vector3& vmin1, const Vector3& vmax1, const Vector3& vmin2, const Vector3& vmax2)
{
	for (int i = 0; i < 3; i++)
	{
		if (!(LessThan(vmin1[i], vmax2[i]) && LessThan(vmin2[i], vmax1[i])))
		{
			return false;
		}
	}
	return true;
}

double volume(const Vector3& a, const Vector3& b)
{
	Vector3 dif = a - b;
	return fabs(dif[0] * dif[1] * dif[2]);
}