#pragma once
#include "../Color.h"
#include "../Intersection.h"
#include "../Math.h"
#include "../Object.h"
#include "../Ray.h"
#include "../Vector.h"

struct CSGSegment
{
	double startTime, endTime;
	bool inside;
	Intersection startIntersection, endIntersection;
};

enum class CSGOperationType
{
	Union,
	Intersection,
	Difference
};

// leftPoints and rightPoints must be sorted by intersection time
void MergeCSGSegments(const Ray& incidentRay, const std::vector<Intersection>& leftPoints, const std::vector<Intersection>& rightPoints, std::vector<Intersection>& mergedPoints, CSGOperationType operation);

class CSGNode
{
public:
	CSGNode(const Shape* child, Transform transform);
	CSGNode(CSGNode* left, CSGNode* right, CSGOperationType operation);
	void GetPoints(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const;

	Vector3 GetMin() const;
	Vector3 GetMax() const;
	~CSGNode() {};
protected:
	CSGNode *left_, *right_;
	const Shape* child_;
	Transform transform_;
	CSGOperationType operation_;
};