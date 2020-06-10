#pragma once
#include "../Materials/Material.h"
#include "../Object.h"
#include "Polygon.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"
	
class ISignedDistanceFunction
{
public:
	virtual double Evaluate(const Vector3& point) const = 0;
	virtual Vector3 GetMin() const = 0;
	virtual Vector3 GetMax() const = 0;
	virtual ~ISignedDistanceFunction() = 0;
};

inline ISignedDistanceFunction::~ISignedDistanceFunction()
{
}

class GridSignedDistanceFunction : public ISignedDistanceFunction
{
private:
	double& get(int i, int j, int k);
	double get(int i, int j, int k) const;
	int x_, y_, z_;
	double* grid_;
	void init();
public:
	GridSignedDistanceFunction(int x, int y, int z, double* cells);
	GridSignedDistanceFunction(std::istream& in);
	~GridSignedDistanceFunction();

	double Evaluate(const Vector3& point) const;
	
	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
};

class SignedDistanceFunctionObject : public Object
{
	Polygon* polygons[6];
public:
	SignedDistanceFunctionObject(ISignedDistanceFunction* function, const Transform& transform, const Material* material);
		
	bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool Contains(const Vector3& point);
	virtual ~SignedDistanceFunctionObject();

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

protected:
	ISignedDistanceFunction* function_;
		
	void gradient(double x, double y, double z, double& gx, double& gy, double& gz) const;
};

