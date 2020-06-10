#pragma once
#include "Math.h"

class Vector2
{
public:
	Vector2() { v[0] = v[1] = 0; }
	Vector2(double x, double y) { v[0] = x; v[1] = y; }
	Vector2(const Vector2& rhs) { v[0] = rhs[0]; v[1] = rhs[1]; }
	Vector2(double vec[2]) { v[0] = vec[0]; v[1] = vec[1]; }
	void Set(double x = 0, double y = 0) { v[0] = x; v[1] = y; }
	double& operator[] (int index) { return v[index]; }
	const double& operator[] (int index) const { return v[index]; }
	operator double* () { return &v[0]; }
	double Dot(const Vector2& rhs) const { return v[0] * rhs[0] + v[1] * rhs[1]; }
	void Normalize() { double z = sqrt(v[0] * v[0] + v[1] * v[1]); if (z != 0) { v[0] /= z; v[1] /= z; } }
	double Magnitude() const { return sqrt(v[0] * v[0] + v[1] * v[1]); }
	Vector2& operator*=(double rhs) { v[0] *= rhs; v[1] *= rhs; return *this; }
	Vector2& operator/=(double rhs) { v[0] /= rhs; v[1] /= rhs; return *this; }
	Vector2 operator*(double rhs) { return Vector2(*this) *= rhs; }
	Vector2 operator/(double rhs) { return Vector2(*this) /= rhs; }
	Vector2& operator+=(Vector2 rhs) { v[0] += rhs[0]; v[1] += rhs[1]; return *this; }
	Vector2& operator-=(Vector2 rhs) { v[0] -= rhs[0]; v[1] -= rhs[1]; return *this; }
	Vector2 operator+(Vector2 rhs) { return Vector2(*this) += rhs; }
	Vector2 operator-(Vector2 rhs) { return Vector2(*this) -= rhs; }
	bool operator<(Vector2 rhs) const { if (v[0] != rhs[0]) return v[0] < rhs[0]; return v[1] < rhs[1]; }
	bool equals(const Vector2& rhs, double dif) { return (*this - rhs).Magnitude() < dif; }
	friend std::ostream& operator<<(std::ostream&, const Vector2&);
	friend std::istream& operator>>(std::istream&, Vector2&);
private:
	double v[2];
};

class Vector3
{
public:
	Vector3() { v[0] = v[1] = v[2] = 0; }
	Vector3(double x, double y, double z) { v[0] = x; v[1] = y; v[2] = z; }
	Vector3(const Vector3& rhs) { v[0] = rhs[0]; v[1] = rhs[1]; v[2] = rhs[2]; }
	Vector3(double vec[3]) { v[0] = vec[0]; v[1] = vec[1]; v[2] = vec[2]; }
	Vector3(const Vector2& rhs) { v[0] = rhs[0]; v[1] = rhs[1]; v[2] = 0; }
	operator Vector2() { return Vector2(v[0], v[1]); }
	void Set(double x = 0, double y = 0, double z = 0) { v[0] = x; v[1] = y; v[2] = z; }
	double& operator[] (int index) { return v[index]; }
	const double& operator[] (int index) const { return v[index]; }
	operator double* () { return &v[0]; }
	operator const double* () { return &v[0]; }
	Vector3 Cross(const Vector3& rhs) const { return Vector3(v[1] * rhs[2] - v[2] * rhs[1], v[2] * rhs[0] - v[0] * rhs[2], v[0] * rhs[1] - v[1] * rhs[0]); }
	double Dot(const Vector3& rhs) const { return v[0] * rhs[0] + v[1] * rhs[1] + v[2] * rhs[2]; }
	Vector3 Reflect(const Vector3& normal) const { return *this - normal * 2 * Dot(normal); }
	void Normalize() { double z = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); if (z != 0) { v[0] /= z; v[1] /= z; v[2] /= z; } }
	// Return a normalized copy
	Vector3 Normalized() const { Vector3 temp(*this); temp.Normalize(); return temp; }
	double Magnitude() const { return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }
	double MagnitudeSquared() const { return v[0] * v[0] + v[1] * v[1] + v[2] * v[2]; }
	Vector3& operator*=(double rhs) { v[0] *= rhs; v[1] *= rhs; v[2] *= rhs; return *this; }
	Vector3& operator/=(double rhs) { v[0] /= rhs; v[1] /= rhs; v[2] /= rhs; return *this; }
	Vector3 operator*(double rhs) const { return Vector3(*this) *= rhs; }
	Vector3 operator/(double rhs) const { return Vector3(*this) /= rhs; }
	Vector3& operator+=(Vector3 rhs) { v[0] += rhs[0]; v[1] += rhs[1]; v[2] += rhs[2]; return *this; }
	Vector3& operator-=(Vector3 rhs) { v[0] -= rhs[0]; v[1] -= rhs[1]; v[2] -= rhs[2]; return *this; }
	Vector3 operator+(const Vector3& rhs) const { return Vector3(*this) += rhs; }
	Vector3 operator-(const Vector3& rhs) const { return Vector3(*this) -= rhs; }
	Vector3 operator-() const { return Vector3(-v[0], -v[1], -v[2]); }
	bool operator<(Vector3 rhs) const { if (v[0] != rhs[0]) return v[0] < rhs[0]; if (v[1] != rhs[1]) return v[1] < rhs[1]; return v[2] < rhs[2]; }
	bool equals(const Vector3& rhs, double dif) const { return (*this - rhs).Magnitude() < dif; }
	friend std::ostream& operator<<(std::ostream&, const Vector3&);
	friend std::istream& operator>>(std::istream&, Vector3&);
private:
	double v[3];
};

inline Vector3 VMin(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(std::min(lhs[0], rhs[0]), std::min(lhs[1], rhs[1]), std::min(lhs[2], rhs[2]));
}

inline Vector3 VMax(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(std::max(lhs[0], rhs[0]), std::max(lhs[1], rhs[1]), std::max(lhs[2], rhs[2]));
}

inline Vector3 operator*(float lhs, const Vector3& rhs)
{
	return Vector3(rhs) *= lhs;
}
inline Vector3 operator*(double lhs, const Vector3& rhs)
{
	return Vector3(rhs) *= lhs;
}

// Returns a random vector in the unit sphere with one edge centered on the origin.
Vector3 RandomInUnitSphere();

// Returns a random vector in the unit disk in the x,y plane, with one edge centered on the origin.
Vector3 RandomInUnitDisk();

// Returns a random vector in the unit sphere, weighted with a cosine distribution, with one edge centered on the origin.
Vector3 RandomCosineDirection();

// Refracts the incoming vector against a surface with given normal vector, return false if total internal reflection.
bool Refract(const Vector3& vector, const Vector3& normal, double ni_over_nt, Vector3& refracted);

class Vector4
{
public:
	Vector4() { v[0] = v[1] = v[2] = v[3] = 0; }
	Vector4(double x, double y, double z, double w) { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }
	Vector4(const Vector4& rhs) { v[0] = rhs[0]; v[1] = rhs[1]; v[2] = rhs[2]; v[3] = rhs[3]; }
	Vector4(const Vector3& rhs, double v4) { v[0] = rhs[0]; v[1] = rhs[1]; v[2] = rhs[2]; v[3] = v4; }
	Vector4(double vec[4]) { v[0] = vec[0]; v[1] = vec[1]; v[2] = vec[2]; v[3] = vec[3]; }

	Vector4(const Vector2& rhs) { v[0] = rhs[0]; v[1] = rhs[1]; v[2] = 0; v[3] = 0; }
	Vector4(const Vector3& rhs) { v[0] = rhs[0]; v[1] = rhs[1]; v[2] = rhs[2]; v[3] = 0; }
	operator Vector3() { return Vector3(v[0], v[1], v[2]); }
	operator Vector2() { return Vector2(v[0], v[1]); }
	void Set(double x = 0, double y = 0, double z = 0, double w = 0) { v[0] = x; v[1] = y; v[2] = z; v[3] = w; }
	double& operator[] (int index) { return v[index]; }
	const double& operator[] (int index) const { return v[index]; }
	operator double* () { return &v[0]; }
	double Dot(const Vector4& rhs) const { return v[0] * rhs[0] + v[1] * rhs[1] + v[2] * rhs[2] + v[3] * rhs[3]; }
	void Normalize() { double z = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]); if (z != 0) { v[0] /= z; v[1] /= z; v[2] /= z; v[3] /= z; } }
	// Return a normalized copy
	Vector4 Normalized() const { Vector4 temp(*this); temp.Normalize(); return temp; }
	double Magnitude() const { return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]); }
	Vector4& operator*=(double rhs) { v[0] *= rhs; v[1] *= rhs; v[2] *= rhs; v[3] *= rhs; return *this; }
	Vector4& operator/=(double rhs) { v[0] /= rhs; v[1] /= rhs; v[2] /= rhs; v[3] /= rhs; return *this; }
	Vector4 operator*(double rhs) const { return Vector4(*this) *= rhs; }
	Vector4 operator/(double rhs) const { return Vector4(*this) /= rhs; }
	Vector4& operator+=(Vector4 rhs) { v[0] += rhs[0]; v[1] += rhs[1]; v[2] += rhs[2]; v[3] += rhs[3]; return *this; }
	Vector4& operator-=(Vector4 rhs) { v[0] -= rhs[0]; v[1] -= rhs[1]; v[2] -= rhs[2]; v[3] -= rhs[3]; return *this; }
	Vector4 operator+(Vector4 rhs) const { return Vector4(*this) += rhs; }
	Vector4 operator-(Vector4 rhs) const { return Vector4(*this) -= rhs; }
	bool equals(Vector4 rhs, double dif) { return (*this - rhs).Magnitude() < dif; }
	friend std::ostream& operator<<(std::ostream&, const Vector4&);
	friend std::istream& operator>>(std::istream&, Vector4&);
private:
	double v[4];
};

std::ostream& operator<<(std::ostream& out, const Vector2& v);
std::istream& operator>>(std::istream& in, Vector2& v);
std::ostream& operator<<(std::ostream& out, const Vector3& v);
std::istream& operator>>(std::istream& in, Vector3& v);
std::ostream& operator<<(std::ostream& out, const Vector4& v);
std::istream& operator>>(std::istream& in, Vector4& v);


bool contains(const Vector3& vmin, const Vector3& vmax, const Vector3& point, double tolerance = EPSILON);
bool contains(const Vector3& vmin1, const Vector3& vmax1, const Vector3& vmin2, const Vector3& vmax2);
double volume(const Vector3& a, const Vector3& b);


void GenerateRandomDirectionInSphere(Vector3& direction);
void GenerateRandomDirectionInHemisphereCosineWeighted(const Vector3& normal, const Vector3& surface, Vector3& direction);