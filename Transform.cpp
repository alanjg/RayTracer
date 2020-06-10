#include "pch.h"
#include "Ray.h"
#include "Transform.h"

void Transform::SetTranslate(const Vector3& translation)
{
	valid_ = false;
	translation_ = translation;
}

void Transform::SetTranslate(double x, double y, double z)
{
	valid_ = false;
	translation_[0] = x;
	translation_[1] = y;
	translation_[2] = z;
}

Vector3 Transform::GetTranslation() const
{
	return translation_;
}

void Transform::SetRotationDeg(const Vector3& axis, double angle)
{
	valid_ = false;
	rotation_.MakeRotateDeg(angle, axis[0], axis[1], axis[2]);
}

void Transform::SetRotationDeg(double axisX, double axisY, double axisZ, double angle)
{
	valid_ = false;
	rotation_.MakeRotateDeg(angle, axisX, axisY, axisZ);
}

Matrix44 Transform::GetRotation() const
{
	return rotation_;
}

void Transform::SetScale(const Vector3& scale)
{
	valid_ = false;
	scale_ = scale;
}

void Transform::SetScale(double x, double y, double z)
{
	valid_ = false;
	scale_[0] = x;
	scale_[1] = y;
	scale_[2] = z;
}

Vector3 Transform::GetScale() const
{
	return scale_;
}

Matrix44 Transform::GetMatrix() const
{
	CheckTransform();
	return transformation_;
}

Ray Transform::TransformLocalToWorld(const Ray& ray) const
{
	CheckTransform();

	Ray result;
	result.origin = transformation_ * Vector4(ray.origin[0], ray.origin[1], ray.origin[2], 1.0);
	result.direction = inverseTranspose_ * Vector4(ray.direction, 0);
	result.direction.Normalize();
	return result;
}

Ray Transform::TransformWorldToLocal(const Ray& ray) const
{
	CheckTransform();

	Ray result;
	result.origin = inverse_ * Vector4(ray.origin, 1);
	result.direction = transpose_ * Vector4(ray.direction, 0);
	result.direction.Normalize();
	return result;
}

Vector3 Transform::TransformLocalPointToWorld(const Vector3& point) const
{
	CheckTransform();
	return transformation_ * Vector4(point, 1);
}

Vector3 Transform::TransformWorldPointToLocal(const Vector3& point) const
{
	CheckTransform();
	return inverse_ * Vector4(point, 1);
}

Vector3 Transform::TransformLocalNormalToWorld(const Vector3& point) const
{
	CheckTransform();
	return transpose_ * Vector4(point, 1).Normalized();
}

Vector3 Transform::TransformWorldNormalToLocal(const Vector3& point) const
{
	CheckTransform();
	return inverseTranspose_ * Vector4(point, 1).Normalized();
}

Transform::Transform()
{
	MakeIdentity();
}

void Transform::MakeIdentity()
{
	valid_ = false;
	rotation_.MakeIdentity();
	translation_ = Vector3(0, 0, 0);
	scale_ = Vector3(1, 1, 1);
}
void Transform::CheckTransform() const
{
	if (valid_)
		return;

	valid_ = true;

	Matrix44 temp;
	transformation_.MakeTranslate(translation_);
	transformation_ *= rotation_;

	temp.MakeScale(scale_);
	transformation_ *= temp;

	transpose_ = transformation_.GetTranspose();
	inverse_ = transformation_.GetInverse();
	inverseTranspose_ = inverse_.GetTranspose();
}