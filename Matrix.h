#pragma once
#include "Vector.h"

class Matrix44
{
public:
	Matrix44();
	Matrix44(double m[4][4]);
	Matrix44(double m[16]);
	Matrix44(Vector4 vec[4]);
	Matrix44(const Matrix44& m);

	double& operator()(int i, int j);
	const double& operator()(int i, int j) const;
	Vector4& operator[](int index);
	const Vector4& operator[](int index) const;

	Matrix44 operator*(const Matrix44&);
	Vector4 operator*(const Vector4&);
	Matrix44 operator*(double f);

	Matrix44& operator*=(const Matrix44&);
	bool operator==(const Matrix44& rhs) const;

	void MakeIdentity();
	void MakeTranslate(const Vector3&);
	void MakeTranslate(double x, double y, double z);
	void MakeRotateDeg(double angle, Vector3 axis);
	void MakeRotateDeg(double angle, double x, double y, double z);
	void MakeRotateRad(double angle, Vector3 axis);
	void MakeRotateRad(double angle, double x, double y, double z);
	void MakeScale(const Vector3&);
	void MakeScale(double x, double y, double z);

	bool Invert(); // returns false if uninvertible
	void Transpose();
	Matrix44 GetInverse();
	Matrix44 GetTranspose();

private:

	double Det2(int i1, int i2, int j1, int j2) const;
	double Det3(int i1, int i2, int i3, int j1, int j2, int j3) const;
	double Det4() const;

	Vector4 v[4];
};

std::ostream& operator<<(std::ostream& out, const Matrix44& v);
std::istream& operator>>(std::istream& in, Matrix44& v);

inline double& Matrix44::operator()(int i, int j)
{
	return v[i][j];
}

inline const double& Matrix44::operator()(int i, int j) const
{
	return v[i][j];
}

inline Vector4& Matrix44::operator[](int index)
{
	return v[index];
}

inline const Vector4& Matrix44::operator[](int index) const
{
	return v[index];
}
