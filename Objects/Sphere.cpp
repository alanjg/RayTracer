#include "pch.h"
#include "Sphere.h"

Vector3 CalcMin(double radius, Transform& transform)
{
	Vector3 box[8];
	int dx[] = { -1,-1,-1,-1,1,1,1,1 };
	int dy[] = { -1,-1,1,1,-1,-1,1,1 };
	int dz[] = { -1,1,-1,1,-1,1,-1,1 };
	for (int i = 0; i < 8; i++)
		box[i] = Vector3(radius * dx[i], radius * dy[i], radius * dz[i]);

	for (int i = 0; i < 8; i++)
	{
		Vector3 b = transform.TransformLocalPointToWorld(box[i]);
		box[i] = b;
	}

	Vector3 vmin = box[0];
	for (int i = 1; i < 8; i++)
		vmin = VMin(vmin, box[i]);
	return vmin;
}

Vector3 CalcMax(double radius, Transform& transform)
{
	Vector3 box[8];
	int dx[] = { -1,-1,-1,-1,1,1,1,1 };
	int dy[] = { -1,-1,1,1,-1,-1,1,1 };
	int dz[] = { -1,1,-1,1,-1,1,-1,1 };
	for (int i = 0; i < 8; i++)
		box[i] = Vector3(radius * dx[i], radius * dy[i], radius * dz[i]);

	for (int i = 0; i < 8; i++)
		box[i] = transform.TransformLocalPointToWorld(box[i]);

	Vector3 vmax = box[0];
	for (int i = 1; i < 8; i++)
		vmax = VMax(vmax, box[i]);
	return vmax;
}

Sphere::Sphere(double radius, const Transform& transform, const Material* material) :
	Object(transform, material)
{
	radius_ = radius;
	vmin_ = CalcMin(radius_, transform_);
	vmax_ = CalcMax(radius_, transform_);
}

Sphere::~Sphere()
{

}

bool Sphere::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	//Solve parametrically
	//ray: x=x0+at,y=y0+bt,z=z0+ct
	//(a,b,c)=theray.direction, (x0,y0-,z0)=theray.origin
	//sphere: x*x+y*y+z*z=r*r, set equal, solve for t
	//imginary roots => no intersection
	//1 root => intersection on surface
	//2 roots => find closest to origin
	Ray localRay = transform_.TransformWorldToLocal(ray);
	Vector3 origin = localRay.origin;
	Vector3 direction = localRay.direction;
	double a = direction.Dot(direction);
	double b = 2.0 * direction.Dot(origin);
	double c = origin.Dot(origin) - radius_ * radius_;
	double disc = b * b - 4 * a * c;

	if (disc < 0.0)
		return false;

	disc = sqrt(disc);
	double t1 = (-b - disc) / (2 * a);
	if (t1 > tMin && t1 < tMax)
	{
		Ray result;
		result.origin = localRay.origin + t1 * localRay.direction;
		result.direction = result.origin / radius_;
		Intersection intersection;
		// ray originated inside the sphere, reverse the normal to point inward
		if (c < INTERSECTION_ADJUSTMENT)
		{
			result.direction *= -1;
			intersection.internalIntersection = true;
		}
		result = transform_.TransformLocalToWorld(result);
		intersection.point = result.origin;
		intersection.normal = result.direction;
		intersection.intersectionTime = t1;
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

	double t2 = (-b + disc) / (2 * a);
	if (t2 > tMin && t2 < tMax)
	{
		Ray result;
		result.origin = localRay.origin + t2 * localRay.direction;
		result.direction = result.origin / radius_;

		Intersection intersection;
		// ray originated inside the sphere, reverse the normal to point inward
		if (c < INTERSECTION_ADJUSTMENT)
		{
			result.direction *= -1;
			intersection.internalIntersection = true;
		}
		result = transform_.TransformLocalToWorld(result);
		intersection.point = result.origin;
		intersection.normal = result.direction;
		intersection.intersectionTime = t2;
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
	return intersectionPoints.size() > 0;
}

bool Sphere::Contains(const Vector3& point)
{
	Vector3 localPoint = transform_.TransformWorldPointToLocal(point);
	return localPoint.Magnitude() <= radius_ + INTERSECTION_ADJUSTMENT;
}

Vector3 Sphere::GetMin() const
{
	return vmin_;
}

Vector3 Sphere::GetMax() const
{
	return vmax_;
}

//shape

SphereShape::SphereShape(double radius)
{
	radius_ = radius;
}

bool SphereShape::GetRoot(const Ray& ray, double a, double b, double c, double disc, double tMin, double tMax, Intersection& intersection) const
{
	double t = (-b - disc) / (2 * a);
	if (t > tMin && t < tMax)
	{
		intersection.point = ray.origin + t * ray.direction;
		intersection.normal = intersection.point / radius_;
		intersection.intersectionTime = t;
		// ray originated inside the sphere, reverse the normal to point inward
		if (LessThan(c, 0))
		{
			intersection.normal *= -1;
			intersection.internalIntersection = true;
		}
		return true;
	}
	return false;
}

bool SphereShape::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	// Solve parametrically
	// ray: x=x0+at,y=y0+bt,z=z0+ct
	// (a,b,c)=theray.direction, (x0,y0-,z0)=theray.origin
	// sphere: x*x+y*y+z*z=r*r, set equal, solve for t
	// imginary roots => no intersection
	// 1 root => intersection on surface
	// 2 roots => find closest to origin
	Vector3 origin = ray.origin;
	Vector3 direction = ray.direction;
	double a = direction.Dot(direction);
	double b = 2.0 * direction.Dot(origin);
	double c = origin.Dot(origin) - radius_ * radius_;
	double discSquared = b * b - 4 * a * c;

	if (discSquared < 0.0)
	{
		return false;
	}

	double disc = sqrt(discSquared);

	Intersection i1, i2;
	bool b1, b2;
	b1 = GetRoot(ray, a, b, c, disc, tMin, tMax, i1);
	b2 = GetRoot(ray, a, b, c, -disc, tMin, tMax, i2);
	
	if (!b1 && !b2)
	{
		return false;
	}

	if (b1 && (!b2 || i1.intersectionTime < i2.intersectionTime))
	{
		firstIntersection = i1;
	}
	else
	{
		firstIntersection = i2;
	}
	if (!firstPointOnly)
	{
		if (b1)
		{
			intersectionPoints.push_back(i1);
		}
		if (b2)
		{
			intersectionPoints.push_back(i2);
		}
	}
	return true;
}

Vector3 SphereShape::GetMin() const
{
	return Vector3(-radius_, -radius_, -radius_);
}

Vector3 SphereShape::GetMax() const
{
	return Vector3(radius_, radius_, radius_);
}