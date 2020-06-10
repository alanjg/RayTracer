#include "pch.h"
#include "Box.h"
#include "Math.h"

bool Box3::Intersect(const Box3& bb) const
{
    for (int i = 0; i < 3; i++)
        if (vmin_[i] > bb.vmax_[i] || vmax_[i] < bb.vmin_[i])
            return false;
    return true;
}

bool Box3::Contains(const Box3& bb) const
{
    for (int i = 0; i < 3; i++)
        if (!(vmin_[i] <= bb.vmin_[i] && vmax_[i] >= bb.vmax_[i]))
            return false;
    return true;
}

bool Box3::Contains(const Vector3& point) const
{
    return vmin_[0] <= point[0] && point[0] <= vmax_[0] &&
        vmin_[1] <= point[1] && point[1] <= vmax_[1] &&
        vmin_[2] <= point[2] && point[2] <= vmax_[2];
}

bool Box3::Intersect(const Ray& ray, double tMin, double tMax) const
{	
	for (int i = 0; i < 3; i++)
	{
		if (Equals(0, ray.direction[i]))
		{
			if (GreaterThan(vmin_[i], ray.origin[i]) || LessThan(vmax_[i], ray.origin[i]))
			{
				return false;
			}
		}
		double t1 = (vmin_[i] - ray.origin[i]) / ray.direction[i];
		double t2 = (vmax_[i] - ray.origin[i]) / ray.direction[i];
		if (ray.direction[i] < 0)
		{
			std::swap(t1, t2);
		}
		tMin = std::max(tMin, t1);
		tMax = std::min(tMin, t2);
		if (tMax <= tMin) return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& out, const Box3& box)
{
    return out << box.vmin_ << " " << box.vmax_ << std::endl;
}
