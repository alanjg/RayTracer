#include "pch.h"
#include "Cylinder.h"

Cylinder::Cylinder(double radius, double height, const Transform& transform, const Material* material):
	Object(transform, material)	
{
	radius_ = radius;
	height_ = height;
}

Cylinder::~Cylinder()
{

}

bool Cylinder::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	firstIntersection.intersectionTime = tMax;

	//solve for top & bottom
	Ray localRay = transform_.TransformWorldToLocal(ray);
	if (localRay.direction[1] != 0)
	{
		double top = (height_ - localRay.origin[1]) / localRay.direction[1];
		double bottom = -localRay.origin[1] / localRay.direction[1];
		
		if (top > 0.0 && (!firstPointOnly || top < firstIntersection.intersectionTime) && top > tMin && top < tMax)
		{
			Ray result;
			result.origin = localRay.origin + top * localRay.direction;
			result.direction = Vector3(0, 1, 0);

			Intersection intersection;
			if (height_ - localRay.origin[1])
			{
				intersection.internalIntersection = true;
				result.direction[1] = -1;
			}

			if (result.origin[0] * result.origin[0] + result.origin[2] * result.origin[2] <= radius_ * radius_)
			{
				result = transform_.TransformLocalToWorld(result);
				intersection.point = result.origin;
				intersection.normal = result.direction;
				intersection.intersectionTime = top;
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
		if (bottom > 0.0 && (!firstPointOnly || bottom < firstIntersection.intersectionTime) && bottom > tMin && bottom < tMax)
		{
			Ray result;
			result.origin = localRay.origin + bottom * localRay.direction;
			result.direction = Vector3(0, -1, 0);
			Intersection intersection;
			intersection.internalIntersection = false;
			if (localRay.origin[1] > 0)
			{
				intersection.internalIntersection = true;
				result.direction[1] = 1;
			}
			if (result.origin[0]*result.origin[0]+result.origin[2]*result.origin[2] <= radius_ * radius_)
			{
				result = transform_.TransformLocalToWorld(result);
				intersection.point = result.origin;
				intersection.normal = result.direction;
				intersection.intersectionTime = bottom;
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

	// now check sides
	Vector3 direction = localRay.direction;
	direction[1] = 0;
	Vector3 origin = localRay.origin;
	origin[1] = 0;
	double a = direction.Dot(direction);
	double b = 2.0 * direction.Dot(origin);
	double c = origin.Dot(origin) - radius_ * radius_;
	double disc = b*b - 4*a*c;
    
	if (disc < 0.0)
	{
		return firstIntersection.intersectionTime < tMax || intersectionPoints.size() > 0;
	}

    disc = sqrt(disc);
    double t1 = (-b-disc) / (2*a);
	if (t1 > 0.0 && (!firstPointOnly || t1 < firstIntersection.intersectionTime) && t1 > tMin && t1 < tMax)
	{
		Intersection intersection;
		intersection.internalIntersection = false;
		Ray result;
		result.origin = localRay.origin + t1 * localRay.direction;
		result.direction = result.origin - Vector3(0, result.origin[1], 0);
		if (c < INTERSECTION_ADJUSTMENT)
		{
			intersection.internalIntersection = true;
			result.direction *= -1;
		}
		result.direction.Normalize();
		if (result.origin[1] > 0 && result.origin[1] < height_)
		{
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
	}

    double t2 = (-b+disc) / (2*a);
	if(t2 > 0.0 && (!firstPointOnly || t2 < firstIntersection.intersectionTime) && t2 > tMin && t2 < tMax)
	{
		Intersection intersection;
		intersection.internalIntersection = false;
		Ray result;
		result.origin = localRay.origin + t2 * localRay.direction;
		result.direction = result.origin - Vector3(0, result.origin[1], 0);
		if (c < INTERSECTION_ADJUSTMENT)
		{
			intersection.internalIntersection = true;
			result.direction *= -1;
		}
		result.direction.Normalize();
		if (result.origin[1] > 0 && result.origin[1] < height_)
		{
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
	}
	return firstIntersection.intersectionTime != tMax || intersectionPoints.size() > 0;
}

bool Cylinder::Contains(const Vector3& point)
{
	Vector3 localPoint = transform_.TransformWorldPointToLocal(point);
	if(localPoint[1] < 0.0 || localPoint[1] > height_)
		return false;
	localPoint[1] = 0.0;
	return localPoint.Magnitude() <= radius_ + INTERSECTION_ADJUSTMENT;
}

Vector3 Cylinder::GetMin() const
{
	Vector3 box[8];
	box[0] = Vector3(-radius_,0,-radius_);
	box[1] = Vector3(radius_,0,-radius_);
	box[2] = Vector3(-radius_,0,radius_);
	box[3] = Vector3(radius_,0,radius_);
	box[4] = Vector3(-radius_,height_,-radius_);
	box[5] = Vector3(radius_,height_,-radius_);
	box[6] = Vector3(-radius_,height_,radius_);
	box[7] = Vector3(radius_,height_,radius_);

	for(int i=0;i<8;i++)
		box[i] = transform_.TransformLocalPointToWorld(box[i]);
	
	Vector3 vmin = box[0];
	for(int i=1;i<8;i++)
		vmin = VMin(vmin,box[i]);
	return vmin;
}

Vector3 Cylinder::GetMax() const
{
	Vector3 box[8];
	box[0] = Vector3(-radius_,0,-radius_);
	box[1] = Vector3(radius_,0,-radius_);
	box[2] = Vector3(-radius_,0,radius_);
	box[3] = Vector3(radius_,0,radius_);
	box[4] = Vector3(-radius_,height_,-radius_);
	box[5] = Vector3(radius_,height_,-radius_);
	box[6] = Vector3(-radius_,height_,radius_);
	box[7] = Vector3(radius_,height_,radius_);

	for(int i=0;i<8;i++)
		box[i] = transform_.TransformLocalPointToWorld(box[i]);
	
	Vector3 vmax = box[0];
	for(int i=1;i<8;i++)
		vmax = VMax(vmax,box[i]);
	return vmax;
}