#pragma once
#include "Ray.h"
#include "Vector.h"

class Box3
{
public:
	Box3() {}
	Box3(const Vector3& vmin, const Vector3& vmax) : vmin_(vmin), vmax_(vmax) {}

	void SetMin(const Vector3& vmin) { vmin_ = vmin; }
	void SetMin(double x, double y, double z) { vmin_[0] = x; vmin_[1] = y; vmin_[2] = z; }
	void SetMax(const Vector3& vmax) { vmax_ = vmax; }
	void SetMax(double x, double y, double z) { vmax_[0] = x; vmax_[1] = y; vmax_[2] = z; }

	void SetBounds(const Vector3& vmin, const Vector3& vmax) { vmin_ = vmin; vmax_ = vmax; }
	void SetBounds(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
	{
		vmin_[0] = xmin; vmin_[1] = ymin; vmin_[2] = zmin; vmax_[0] = xmax; vmax_[1] = ymax; vmax_[2] = zmax;
	}

	void Extend(const Vector3& point)
	{
		vmin_[0] = std::min(vmin_[0], point[0]);
		vmin_[1] = std::min(vmin_[1], point[1]);
		vmin_[2] = std::min(vmin_[2], point[2]);

		vmax_[0] = std::max(vmax_[0], point[0]);
		vmax_[1] = std::max(vmax_[1], point[1]);
		vmax_[2] = std::max(vmax_[2], point[2]);
	}

	void GetBounds(Vector3& vmin, Vector3& vmax) { vmin = vmin_; vmax = vmax_; }
	void GetBounds(double& xmin, double& ymin, double& zmin, double& xmax, double& ymax, double& zmax)
	{
		xmin = vmin_[0]; ymin = vmin_[1]; zmin = vmin_[2]; xmax = vmax_[0]; ymax = vmax_[1]; zmax = vmax_[2];
	}

	const Vector3& GetMin() const { return vmin_; }
	const Vector3& GetMax() const { return vmax_; }
	Vector3& GetMin() { return vmin_; }
	Vector3& GetMax() { return vmax_; }

	Vector3 GetCenter() const { return (vmin_ + vmax_) / 2.0; }

	//intersect with another box
	bool Intersect(const Box3& bb) const;

	//true if *this contains bb
	bool Contains(const Box3& bb) const;

	//intersect with a point
	bool Contains(const Vector3& point) const;

	bool Intersect(const Ray& ray, double tMin, double tMax) const;

	friend std::ostream& operator<<(std::ostream& out, const Box3& box);
private:
	Vector3 vmin_, vmax_;
};