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
	CSGNode(CSGNode* left, CSGNode* right);
	CSGNode(IObject* child);
	CSGNode(CSGNode* left, CSGNode* right, CSGOperationType operation);
	virtual void GetPoints(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const;
	void GetPointsEx(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const;
	virtual bool Contains(const Vector3& point);

	virtual Vector3 GetMin() const;
	virtual Vector3 GetMax() const;
	virtual ~CSGNode() {};
protected:
	CSGNode *left_, *right_;
	IObject* child_;
	CSGOperationType operation_;
};