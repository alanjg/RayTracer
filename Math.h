#pragma once

const double EPSILON = 1e-8;
const double INTERSECTION_ADJUSTMENT = 1e-3;
const double T_MIN = 1e-3;
const double T_MAX = std::numeric_limits<double>::max();

double DEG2RAD(double degrees);
double RAD2DEG(double radians);
bool LessThan(double a, double b, double tolerance = EPSILON);
bool GreaterThan(double a, double b, double tolerance = EPSILON);
bool Equals(double a, double b, double tolerance = EPSILON);
double Schlick(double cosine, double refractionIndex);

#define FAST_RANDOM

class Random
{
private:
#ifdef FAST_RANDOM

#else
	static std::uniform_real_distribution<double> distribution_;
	static std::default_random_engine generator_;
#endif // FAST_RANDOM

public:
	static double NextReal();
};