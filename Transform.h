#pragma once
#include "Matrix.h"
#include "Ray.h"
#include "Vector.h"

class Transform
{
public:
	void SetTranslate(const Vector3& translation);
	void SetTranslate(double x, double y, double z);
	Vector3 GetTranslation() const;
	void SetRotationDeg(const Vector3& axis, double angle);
	void SetRotationDeg(double axisX, double axisY, double axisZ, double angle);
	Matrix44 GetRotation() const;
	void SetScale(const Vector3& scale);
	void SetScale(double x, double y, double z);
	Vector3 GetScale() const;
	Matrix44 GetMatrix() const;
	Ray TransformLocalToWorld(const Ray& ray) const;
	Ray TransformWorldToLocal(const Ray& ray) const;

	Vector3 TransformLocalPointToWorld(const Vector3& point) const;
	Vector3 TransformWorldPointToLocal(const Vector3& point) const;

	Vector3 TransformLocalNormalToWorld(const Vector3& point) const;
	Vector3 TransformWorldNormalToLocal(const Vector3& point) const;

	Transform();

	void MakeIdentity();
private:
	Vector3 translation_;
	Matrix44 rotation_;
	Vector3 scale_;

	mutable Matrix44 transformation_;
	mutable Matrix44 transpose_;
	mutable Matrix44 inverse_;
	mutable Matrix44 inverseTranspose_;
	mutable bool valid_;

	void CheckTransform() const;
};