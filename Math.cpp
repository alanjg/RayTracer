#include "pch.h"
#include "Math.h"

double DEG2RAD(double degrees)
{
	return degrees * M_PI / 180.0;
}

double RAD2DEG(double radians)
{
	return radians * 180.0 / M_PI;
}

bool LessThan(double a, double b, double tolerance)
{
	return a - b < tolerance;
}

bool GreaterThan(double a, double b, double tolerance)
{
	return b - a < tolerance;
}

bool Equals(double a, double b, double tolerance)
{
	return fabs(a - b) < tolerance;
}

double Schlick(double cosine, double refractionIndex)
{
	double r = (1 - refractionIndex) / (1 + refractionIndex);
	r = r * r;
	return r + (1 - r) * pow((1 - cosine), 5);
}

#ifdef FAST_RANDOM

#else

std::uniform_real_distribution<double> Random::distribution_;
std::default_random_engine Random::generator_;

#endif // FAST_RANDOM

double Random::NextReal()
{
#ifdef FAST_RANDOM
	return ((double)rand()) / RAND_MAX;
#else
	return Random::distribution_(Random::generator_);
#endif
}