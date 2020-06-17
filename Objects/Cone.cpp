#include "pch.h"
#include "Cone.h"

Cone::Cone(double bottomRadius, double topRadius, double height) :
	bottomRadius_(bottomRadius), topRadius_(topRadius), height_(height)
{
}

bool Cone::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	firstIntersection.distance = tMax;
	//solve for top & bottom
	if (ray.direction[1] != 0)
	{
		double top = (height_ - ray.origin[1]) / ray.direction[1];
		double bottom = -ray.origin[1] / ray.direction[1];

		if (top > 0.0)
		{
			Vector3 point = ray.origin + top * ray.direction;
			if (point[0] * point[0] + point[2] * point[2] <= topRadius_ * topRadius_ && top > tMin && top < tMax)
			{
				Intersection intersection;
				intersection.point = point;
				intersection.normal.Set(0, 1, 0);
				intersection.distance = top;
				if (ray.direction[1] > 0)
				{
					intersection.internalIntersection = true;
				}
				if (firstPointOnly)
				{
					firstIntersection = intersection;
				}
				else
				{
					intersectionPoints.push_back(intersection);
				}
			}
		}
		if (bottom > 0.0 && (!firstPointOnly || bottom < firstIntersection.distance))
		{
			Vector3 point = ray.origin + bottom * ray.direction;
			if (point[0] * point[0] + point[2] * point[2] <= bottomRadius_ * bottomRadius_ && bottom > tMin && bottom < tMax)
			{
				Intersection intersection;
				intersection.point = point;
				intersection.normal.Set(0, -1, 0);
				intersection.distance = bottom;
				if (ray.direction[1] < 0)
				{
					intersection.internalIntersection = true;
				}
				if (firstPointOnly)
				{
					firstIntersection = intersection;
				}
				else
				{
					intersectionPoints.push_back(intersection);
				}
			}
		}
	}

	//cone
	//x^2 + z^2 = (Rtop*(y/y_top)+Rbottom*(ytop-y)/ytop)^2
	//x = x0 + at
	//y = y0 + bt
	//z = z0 + ct
	//(x0 + at)^2 + (z0 + ct)^2 - (Rtop*(y0 + bt)/y_top + Rbottom*(y_top - y0 - bt)/ytop)^2
	//x0^2 + 2x0 * at + a^2t^2 + z0^2 + 2z0*ct + c^2t^2 - Rpart
	//D = rtop*y0/ytop
	//E = Rtop * b/ytop
	//F = Rbottom* y0 /ytop
	//G = Rbottom * b/ytop
	//Rpart = (D + Et + Rbottom - F - Gt)^2
	//H = D - F + Rbottom
	//I = E - G
	//Rpart = (H + It)^2
	//RPart = H^2 + 2HIt + I^2t^2
	//x0^2 + 2x0 * at + a^2t^2 + z0^2 + 2z0*ct + c^2t^2 - H^2 - 2HIt - I^2t^2
	//= t^2(a^2 + c^2 + I^2) + t(2x0 + 2z0 - 2HI) + 1(x0^2 + z0^2 - H^2)
	//A = a^2 + c^2 - I ^2
	//B = 2x0 + 2z0 - 2HI
	//C = x0^2 + z0^2 - H^2
	double D = topRadius_ * ray.origin[1] / height_;
	double E = topRadius_ * ray.direction[1] / height_;
	double F = bottomRadius_ * ray.origin[1] / height_;
	double G = bottomRadius_ * ray.direction[1] / height_;
	double H = D - F + bottomRadius_;
	double I = E - G;

	Vector3 direction = ray.direction;
	direction[1] = 0;
	Vector3 origin = ray.origin;
	origin[1] = 0;
	double a = direction.Dot(direction) - I * I;
	double b = 2.0 * direction.Dot(origin) - 2 * H * I;
	double c = origin.Dot(origin) - H * H;
	double disc = b * b - 4 * a * c;

	if (disc < 0.0)
	{
		return firstIntersection.distance < tMin || intersectionPoints.size() > 0;
	}

	double angle = -(topRadius_ - bottomRadius_) / height_;
	Vector3 normal(1, angle, 0);
	normal.Normalize();
	double incline = normal[1];

	disc = sqrt(disc);
	double t1 = (-b - disc) / (2 * a);
	if (t1 > 0.0 && (!firstPointOnly || t1 < firstIntersection.distance) && t1 > tMin && t1 < tMax)
	{
		Vector3 point = ray.origin + t1 * ray.direction;
		Vector3 normal = point - Vector3(0, point[1], 0);
		normal.Normalize();
		double mag = sqrt(1.0 - incline * incline);
		normal *= mag;
		normal[1] = incline;
		if (point[1] > 0 && point[1] < height_)
		{
			Intersection intersection;
			intersection.point = point;
			intersection.normal = normal;
			intersection.distance = t1;
			if (normal.Dot(ray.direction) > 0)
			{
				intersection.internalIntersection = true;
			}
			if (firstPointOnly)
			{
				firstIntersection = intersection;
				return true;
			}
			else
			{
				intersectionPoints.push_back(intersection);
			}
		}
	}

	double t2 = (-b + disc) / (2 * a);
	if (t2 > 0.0 && (!firstPointOnly || t2 < firstIntersection.distance) && t2 > tMin && t2 < tMax)
	{
		Vector3 point = ray.origin + t2 * ray.direction;
		Vector3 normal = point - Vector3(0, point[1], 0);
		normal.Normalize();
		double mag = sqrt(1.0 - incline);
		normal *= mag;
		normal[1] = incline;

		if (point[1] > 0 && point[1] < height_)
		{
			Intersection intersection;
			intersection.point = point;
			intersection.normal = normal;
			intersection.distance = t2;
			if (normal.Dot(ray.direction) > 0)
			{
				intersection.internalIntersection = true;
			}
			if (firstPointOnly)
			{
				firstIntersection = intersection;
				return true;
			}
			else
			{
				intersectionPoints.push_back(intersection);
			}

		}
	}
	return firstIntersection.distance < tMin || intersectionPoints.size() > 0;
}

Vector3 Cone::GetMin() const
{
	return Vector3(std::min(-bottomRadius_, -topRadius_), 0, std::min(-bottomRadius_, -topRadius_));
}

Vector3 Cone::GetMax() const
{
	return Vector3(std::max(bottomRadius_, topRadius_), height_, std::max(bottomRadius_, topRadius_));
}