#pragma once
#include "../Materials/Material.h"
#include "../Box.h"
#include "../Object.h"
#include "../Intersection.h"
#include "../Ray.h"
#include "../Transform.h"

class TriangleMesh : public Shape
{
	std::vector<Vector3> vertices_;
	std::vector<int> indices_;

public:
	TriangleMesh(std::vector<Vector3> vertices, std::vector<int> indices);
	~TriangleMesh();
	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;

private:
	virtual bool GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const;
	bool PointInTriangle(const Vector3& point, int faceIndex) const;
	Box3 bounds_;
};