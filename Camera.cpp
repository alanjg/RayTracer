#include "pch.h"
#include "Camera.h"


void Camera::SetViewport(int w, int h)
{
	Vector3 fup = up;
	fup.Normalize();

	Vector3 dir = centerOfInterest - position;
	double dist = dir.Magnitude();
	dir.Normalize();

	double aspectRatio = float(w) / float(h);
	double yScale = tan(fov / 2.0 * 3.14159f / 180.0f);
	double xScale = yScale * aspectRatio;

	Vector3 right = dir.Cross(fup);
	right.Normalize();

	//adjust up to ensure it's orthogonal
	fup = right.Cross(dir);
	fup.Normalize();

	double hOffset = yScale * dist;
	double wOffset = xScale * dist;

	eyePosition_ = position;
	vdx_ = right * wOffset * 2;
	vdy_ = -fup * hOffset * 2;
	upperLeftBound_ = centerOfInterest + fup * hOffset - right * wOffset;
	width_ = w;
	height_ = h;
}


Ray Camera::CreateRay(int x, int y)
{
	Ray ray;
	ray.origin = eyePosition_;
	double xd = (double)x / (double)width_;
	double yd = (double)y / (double)height_;
	Vector3 target = upperLeftBound_ + vdx_ * xd + vdy_ * yd;

	ray.direction = target - eyePosition_;
	ray.direction.Normalize();
	return ray;
}

Ray Camera::CreateRandomRayOld(int x, int y)
{
	Ray ray;
	ray.origin = eyePosition_;
	double xd = (double)(x + Random::NextReal()) / (double)width_;
	double yd = (double)(y + Random::NextReal()) / (double)height_;
	Vector3 target = upperLeftBound_ + vdx_ * xd + vdy_ * yd;

	ray.direction = target - eyePosition_;
	ray.direction.Normalize();
	return ray;
}

Ray Camera::CreateRandomRay(int x, int y)
{
	Ray ray;
	Vector3 randomOnLens = aperture / 2.0 * RandomInUnitDisk();
	Vector3 vdxn = vdx_;
	vdxn.Normalize();
	Vector3 vdyn = vdy_;
	vdyn.Normalize();
	Vector3 focusOffset = randomOnLens[0] * vdxn + randomOnLens[1] * vdyn;
	ray.origin = eyePosition_ + focusOffset;
	double xd = (double)(x + Random::NextReal()) / (double)width_;
	double yd = (double)(y + Random::NextReal()) / (double)height_;
	Vector3 target = upperLeftBound_ + vdx_ * xd + vdy_ * yd;

	ray.direction = target - ray.origin;
	ray.direction.Normalize();
	return ray;
}