#include "pch.h"
#include "Ellipsoid.h"

Ellipsoid::Ellipsoid(const Vector3& radii, const Transform& transform, const Material* material):
	Object(transform, material), radii_(radii)
{
	
}

Ellipsoid::~Ellipsoid()
{

}

bool Ellipsoid::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	//Solve parametrically
    //ray: x=x0+at,y=y0+bt,z=z0+ct
    //(a,b,c)=theray.direction, (x0,y0,z0)=theray.origin
    //Ellipsoid: x*x/rx*rx+y*y/ry*ry+z*z/rz*rz=1, set equal, solve for t
    //imginary roots => no intersection
    //1 root => intersection on surface
    //2 roots => find closest to origin

	Ray localRay = transform_.TransformWorldToLocal(ray);
	Vector3 origin = localRay.origin;
	Vector3 direction = localRay.direction;
    Vector3 scaledOrigin(origin);
	Vector3 scaledDirection(direction);
	scaledOrigin[0] /= radii_[0];
	scaledOrigin[1] /= radii_[1];
	scaledOrigin[2] /= radii_[2];
	scaledDirection[0] /= radii_[0];
	scaledDirection[1] /= radii_[1];
	scaledDirection[2] /= radii_[2];

	double a = scaledDirection.Dot(scaledDirection);
    double b = 2.0 * scaledDirection.Dot(scaledOrigin);
    double c = scaledOrigin.Dot(scaledOrigin) - 1;
    double disc = b*b - 4*a*c;    
	if (disc < 0.0)
	{
		return false;
	}

    disc = sqrt(disc);
    double t1 = (-b-disc) / (2*a);
    if(t1 > 0.0 && t1 > tMin && t1 < tMax) 
	{
		Ray result;
		result.origin = localRay.origin + t1 * localRay.direction;
		
		result.direction = result.origin;
		result.direction[0] /= radii_[0]*radii_[0];
		result.direction[1] /= radii_[1]*radii_[1];
		result.direction[2] /= radii_[2]*radii_[2];
		result.direction.Normalize();

		result = transform_.TransformLocalToWorld(result);
		Intersection intersection;
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

    double t2 = (-b+disc) / (2*a);
	if (t2 > 0.0 && (!firstPointOnly || t2 < firstIntersection.intersectionTime) && t2 > tMin && t2 < tMax)
	{
		Ray result;
		result.origin = localRay.origin + t2 * localRay.direction;
		
		result.direction = result.origin;
		result.direction[0] /= radii_[0]*radii_[0];
		result.direction[1] /= radii_[1]*radii_[1];
		result.direction[2] /= radii_[2]*radii_[2];
		result.direction.Normalize();

		result = transform_.TransformLocalToWorld(result);
		Intersection intersection;
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

bool Ellipsoid::Contains(const Vector3& point)
{
	Vector3 localPoint = transform_.TransformWorldPointToLocal(point);
	localPoint[0] /= radii_[0];
	localPoint[1] /= radii_[1];
	localPoint[2] /= radii_[2];
	return localPoint.Dot(localPoint) <= 1 + INTERSECTION_ADJUSTMENT;
}

Vector3 Ellipsoid::GetMin() const
{
	Vector3 box[8];
	int dx[]={-1,-1,-1,-1,1,1,1,1};
	int dy[]={-1,-1,1,1,-1,-1,1,1};
	int dz[]={-1,1,-1,1,-1,1,-1,1};
	for(int i=0;i<8;i++)
		box[i] = Vector3(radii_[0]*dx[i],radii_[1]*dy[i],radii_[2]*dz[i]);

	for(int i=0;i<8;i++)
		box[i] = transform_.TransformLocalPointToWorld(box[i]);
	
	Vector3 vmin = box[0];
	for(int i=1;i<8;i++)
		vmin = VMin(vmin,box[i]);
	return vmin;
}

Vector3 Ellipsoid::GetMax() const
{
	Vector3 box[8];
	int dx[]={-1,-1,-1,-1,1,1,1,1};
	int dy[]={-1,-1,1,1,-1,-1,1,1};
	int dz[]={-1,1,-1,1,-1,1,-1,1};
	for(int i=0;i<8;i++)
		box[i] = Vector3(radii_[0]*dx[i],radii_[1]*dy[i],radii_[2]*dz[i]);

	for(int i=0;i<8;i++)
		box[i] = transform_.TransformLocalPointToWorld(box[i]);
	
	Vector3 vmax = box[0];
	for(int i=1;i<8;i++)
		vmax = VMax(vmax,box[i]);
	return vmax;
}