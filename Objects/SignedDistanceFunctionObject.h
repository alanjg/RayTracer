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

class SignedDistanceFunctionShape : public Shape
{
	Polygon* polygons[6];
public:
	SignedDistanceFunctionShape(ISignedDistanceFunction* function);

	bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	
	virtual ~SignedDistanceFunctionShape();

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

protected:
	ISignedDistanceFunction* function_;

	Vector3 Gradient(Vector3 position) const;
};

