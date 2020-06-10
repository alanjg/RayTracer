#include "pch.h"
#include "CSGNode.h"

struct reverse_normal 
{
	void operator()(Intersection& i)
	{
		i.normal *= -1;
		i.internalIntersection = !i.internalIntersection;
	}
};

bool csgOperation(bool lhs, bool rhs, CSGOperationType operation)
{
	if (operation == CSGOperationType::Union)
	{
		return lhs || rhs;
	}
	else if (operation == CSGOperationType::Intersection)
	{
		return lhs && rhs;
	}
	else if (operation == CSGOperationType::Difference)
	{
		return lhs && !rhs;
	}
	else
	{
		return false;
	}
}
	
CSGNode::CSGNode(CSGNode* left, CSGNode* right):
	left_(left),right_(right)
{
}

CSGNode::CSGNode(IObject* child) :
	left_(nullptr), right_(nullptr), child_(child), operation_(CSGOperationType::Union)
{
}

CSGNode::CSGNode(CSGNode* left, CSGNode* right, CSGOperationType operation) :
	left_(left), right_(right), child_(nullptr), operation_(operation)
{
}

void CSGNode::GetPoints(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const
{

}

bool CSGNode::Contains(const Vector3& point)
{
	if (child_ != nullptr)
	{
		return child_->Contains(point);
	}
	else if (operation_ == CSGOperationType::Union)
	{
		return left_->Contains(point) || right_->Contains(point);
	}
	else if (operation_ == CSGOperationType::Intersection)
	{
		return left_->Contains(point) && right_->Contains(point);
	}
	else if (operation_ == CSGOperationType::Difference)
	{
		return left_->Contains(point) && !right_->Contains(point);
	}
	else
	{
		// should not get here
		return false;
	}
}

Vector3 CSGNode::GetMin() const
{
	if (child_ != nullptr)
	{
		return child_->GetMin();
	}
	else
	{
		return VMin(left_->GetMin(), right_->GetMin());
	}
}

Vector3 CSGNode::GetMax() const
{
	if (child_ != nullptr)
	{
		return child_->GetMax();
	}
	else
	{
		return VMax(left_->GetMax(), right_->GetMax());
	}
}

void CSGNode::GetPointsEx(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const
{
	if (child_ != nullptr)
	{
		child_->GetPoints(ray, points, tMin, tMax);
	}
	else
	{
		std::vector<Intersection> p1, p2;
		left_->GetPointsEx(ray, p1, tMin, tMax);
		right_->GetPointsEx(ray, p2, tMin, tMax);
		std::sort(p1.begin(), p1.end());
		std::sort(p2.begin(), p2.end());
		MergeCSGSegments(ray, p1, p2, points, operation_);
	}	
}

class CSGContains
{
	CSGNode* obj_;
public:
	CSGContains(CSGNode* obj): obj_(obj) {}
	bool operator()(const Intersection& intersection) const
	{
		return obj_->Contains(intersection.point);
	}
};

class PointListContains
{
	std::vector<Intersection>& intersections_;
public:
	PointListContains(std::vector<Intersection>& intersections) : intersections_(intersections) {}
	bool operator()(const Intersection& intersection) const
	{
		for (auto x : intersections_)
		{
			if ((x.point - intersection.point).MagnitudeSquared() < EPSILON*EPSILON)
			{
				return true;
			}
		}
		return false;
	}
}; 

// leftPoints and rightPoints must be sorted by intersection time
void MergeCSGSegments(const Ray& incidentRay, const std::vector<Intersection>& leftPoints, const std::vector<Intersection>& rightPoints, std::vector<Intersection>& mergedPoints, CSGOperationType operation)
{
	assert(std::is_sorted(leftPoints.begin(), leftPoints.end()));
	assert(std::is_sorted(rightPoints.begin(), rightPoints.end()));

	int leftAt = leftPoints.size() - 1;
	int rightAt = rightPoints.size() - 1;
	double t = T_MAX;
	bool leftInside = false, rightInside = false;

	// Build segments by processing intersection points.  The first segment has no start intersection and the last segment has no end intersection.
	std::vector<CSGSegment> tempSegments;
	CSGSegment currentSegment;
	currentSegment.endTime = t;
	currentSegment.inside = false;
	bool hasSegment = false;
	while (leftAt >= 0 || rightAt >= 0)
	{
		currentSegment.inside = csgOperation(leftInside, rightInside, operation);
		if (leftAt >= 0 && rightAt >= 0 && Equals(leftPoints[leftAt].intersectionTime, rightPoints[rightAt].intersectionTime))
		{
			// overlapping intersection, take the left one arbitrarily and advance both
			currentSegment.startTime = leftPoints[leftAt].intersectionTime;
			currentSegment.startIntersection = leftPoints[leftAt];
			leftInside = !leftInside;
			rightInside = !rightInside;
			leftAt--;
			rightAt--;
		}
		else if (leftAt >= 0 && (rightAt == -1 || leftPoints[leftAt].intersectionTime > rightPoints[rightAt].intersectionTime))
		{
			currentSegment.startTime = leftPoints[leftAt].intersectionTime;
			currentSegment.startIntersection = leftPoints[leftAt];
			leftInside = !leftInside;
			leftAt--;
		}
		else
		{
			currentSegment.startTime = rightPoints[rightAt].intersectionTime;
			currentSegment.startIntersection = rightPoints[rightAt];
			rightInside = !rightInside;
			rightAt--;
		}
		t = currentSegment.startTime;
		tempSegments.push_back(currentSegment);
		currentSegment.endTime = t;
		currentSegment.endIntersection = currentSegment.startIntersection;
	}
	currentSegment.inside = csgOperation(leftInside, rightInside, operation);
	currentSegment.startTime = 0;
	currentSegment.startIntersection.hitObject = nullptr;
	tempSegments.push_back(currentSegment);

	for (int i = tempSegments.size() - 1; i > 0; i--)
	{
		int j = i - 1;
		while (j >= 0 && tempSegments[j].inside == tempSegments[i].inside)
		{
			j--;
		}
		j++;
		// never add the last segment start intersection - it doesn't exist.
		if (j > 0)
		{
			Intersection mergedIntersection = tempSegments[j].endIntersection;
			// Fix intersection normal and inside status
			mergedIntersection.internalIntersection = tempSegments[j].inside;
			if (mergedIntersection.normal.Dot(incidentRay.direction) > 0)
			{
				mergedIntersection.normal *= -1;
			}
			mergedPoints.push_back(mergedIntersection);
		}
		i = j;
	}
}