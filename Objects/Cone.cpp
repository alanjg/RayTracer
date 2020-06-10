#include "pch.h"
#include "Cone.h"

Cone::Cone(double bottomRadius, double topRadius, double height, const Transform& transform, const Material* material):
	Object(transform, material)	
{
	bottomRadius_ = bottomRadius;
	topRadius_ = topRadius;
	height_ = height;
}

Cone::~Cone()
{

}

bool Cone::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	firstIntersection.intersectionTime = tMax;
	//solve for top & bottom
	Ray localRay = transform_.TransformWorldToLocal(ray);
	if (localRay.direction[1] != 0)
	{
		double top = (height_ - localRay.origin[1])/localRay.direction[1];
		double bottom = -localRay.origin[1]/localRay.direction[1];
		
		if (top > 0.0)
		{
			Ray result;
			result.origin = localRay.origin + top * localRay.direction;
			result.direction = Vector3(0,1,0);
			if (result.origin[0] * result.origin[0] + result.origin[2] * result.origin[2] <= topRadius_ * topRadius_ && top > tMin && top < tMax)
			{
				result = transform_.TransformLocalToWorld(result);
				Intersection intersection;
				intersection.point = result.origin;
				intersection.normal = result.direction;
				intersection.intersectionTime = top;
				if (localRay.direction[1] > 0)
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
		if (bottom > 0.0 && (!firstPointOnly || bottom < firstIntersection.intersectionTime))
		{
			Ray result;
			result.origin = localRay.origin + bottom * localRay.direction;
			result.direction = Vector3(0,-1,0);
			if (result.origin[0] * result.origin[0] + result.origin[2] * result.origin[2] <= bottomRadius_ * bottomRadius_ && bottom > tMin && bottom < tMax)
			{
				result = transform_.TransformLocalToWorld(result);
				Intersection intersection;
				intersection.point = result.origin;
				intersection.normal = result.direction;
				intersection.intersectionTime = bottom;
				if (localRay.direction[1] < 0)
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
	double D = topRadius_ * localRay.origin[1] / height_;
	double E = topRadius_ * localRay.direction[1] / height_;
	double F = bottomRadius_ * localRay.origin[1] / height_;
	double G = bottomRadius_ * localRay.direction[1] / height_;
	double H = D - F + bottomRadius_;
	double I = E - G;
	
	Vector3 direction = localRay.direction;
	direction[1] = 0;
	Vector3 origin = localRay.origin;
	origin[1] = 0;
	double a = direction.Dot(direction) - I * I;
	double b = 2.0 * direction.Dot(origin) - 2 * H * I;	
	double c = origin.Dot(origin) - H * H;
	double disc = b*b - 4*a*c;
    
	if (disc < 0.0)
	{
		return firstIntersection.intersectionTime < tMin || intersectionPoints.size() > 0;
	}

	double angle = -(topRadius_ - bottomRadius_) / height_;
	Vector3 normal(1,angle,0);
	normal.Normalize();
	double incline = normal[1];

    disc = sqrt(disc);
    double t1 = (-b-disc) / (2*a);
	if (t1 > 0.0 && (!firstPointOnly || t1 < firstIntersection.intersectionTime) && t1 > tMin && t1 < tMax)
	{
		Ray localResult;
		localResult.origin = localRay.origin + t1 * localRay.direction;
		localResult.direction = localResult.origin - Vector3(0, localResult.origin[1], 0);
		localResult.direction.Normalize();
		double mag = sqrt(1.0 - incline*incline);
		localResult.direction *= mag;
		localResult.direction[1] = incline;
		if(localResult.origin[1] > 0 && localResult.origin[1] < height_)
		{
			Ray worldResult = transform_.TransformLocalToWorld(localResult);
			Intersection intersection;
			intersection.point = worldResult.origin;
			intersection.normal = worldResult.direction;
			intersection.intersectionTime = t1;
			if (localResult.direction.Dot(localRay.direction) > 0)
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

    double t2 = (-b+disc) / (2*a);
	if (t2 > 0.0 && (!firstPointOnly || t2 < firstIntersection.intersectionTime) && t2 > tMin && t2 < tMax)
	{
		Ray localResult;
		localResult.origin = localRay.origin + t2 * localRay.direction;
		localResult.direction = localResult.origin - Vector3(0, localResult.origin[1], 0);
		localResult.direction.Normalize();
		
		double mag = sqrt(1.0 - incline);
		localResult.direction *= mag;
		localResult.direction[1] = incline;

		if(localResult.origin[1] > 0 && localResult.origin[1] < height_)
		{
			Ray worldResult = transform_.TransformLocalToWorld(localResult);
			Intersection intersection;
			intersection.point = worldResult.origin;
			intersection.normal = worldResult.direction;
			intersection.intersectionTime = t2;
			if (localResult.direction.Dot(localRay.direction) > 0) 
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
	return firstIntersection.intersectionTime < tMin || intersectionPoints.size() > 0;
}

bool Cone::Contains(const Vector3& point)
{
	Vector3 localPoint = transform_.TransformWorldPointToLocal(point);
	if(localPoint[1] < 0.0 || localPoint[1] > height_)
		return false;
	double radius = bottomRadius_ + (topRadius_ - bottomRadius_) * localPoint[1] / height_;
	localPoint[1] = 0.0;
	return localPoint.Magnitude() <= radius + INTERSECTION_ADJUSTMENT;
}

Vector3 Cone::GetMin() const
{
	Vector3 box[8];
	box[0] = Vector3(-bottomRadius_,0,-bottomRadius_);
	box[1] = Vector3(bottomRadius_,0,-bottomRadius_);
	box[2] = Vector3(-bottomRadius_,0,bottomRadius_);
	box[3] = Vector3(bottomRadius_,0,bottomRadius_);
	box[4] = Vector3(-topRadius_,height_,-topRadius_);
	box[5] = Vector3(topRadius_,height_,-topRadius_);
	box[6] = Vector3(-topRadius_,height_,topRadius_);
	box[7] = Vector3(topRadius_,height_,topRadius_);

	for(int i=0;i<8;i++)
		box[i] = transform_.TransformLocalPointToWorld(box[i]);
	
	Vector3 vmin = box[0];
	for(int i=1;i<8;i++)
		vmin = VMin(vmin,box[i]);
	return vmin;
}

Vector3 Cone::GetMax() const
{
	Vector3 box[8];
	box[0] = Vector3(-bottomRadius_,0,-bottomRadius_);
	box[1] = Vector3(bottomRadius_,0,-bottomRadius_);
	box[2] = Vector3(-bottomRadius_,0,bottomRadius_);
	box[3] = Vector3(bottomRadius_,0,bottomRadius_);
	box[4] = Vector3(-topRadius_,height_,-topRadius_);
	box[5] = Vector3(topRadius_,height_,-topRadius_);
	box[6] = Vector3(-topRadius_,height_,topRadius_);
	box[7] = Vector3(topRadius_,height_,topRadius_);

	for(int i=0;i<8;i++)
		box[i] = transform_.TransformLocalPointToWorld(box[i]);
	
	Vector3 vmax = box[0];
	for(int i=1;i<8;i++)
		vmax = VMax(vmax,box[i]);
	return vmax;
}