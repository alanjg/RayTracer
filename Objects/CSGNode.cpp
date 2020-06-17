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

CSGNode::CSGNode(const Shape* childShape, Transform transform) :
	left_(nullptr), right_(nullptr), child_(childShape), transform_(transform), operation_(CSGOperationType::Union)
{
}

CSGNode::CSGNode(CSGNode* left, CSGNode* right, CSGOperationType operation) :
	left_(left), right_(right), child_(nullptr), operation_(operation)
{
}

Vector3 CSGNode::GetMin() const
{
	if (child_ != nullptr)
	{
		return transform_.TransformLocalPointToWorld(child_->GetMin());
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
		return transform_.TransformLocalPointToWorld(child_->GetMax());
	}
	else
	{
		return VMax(left_->GetMax(), right_->GetMax());
	}
}

void CSGNode::GetPoints(const Ray& ray, std::vector<Intersection>& points, double tMin, double tMax) const
{
	if (child_ != nullptr)
	{
		Ray localRay = transform_.TransformWorldToLocal(ray);
		child_->GetPoints(localRay, points, tMin, tMax);
		for (Intersection& intersection : points)
		{
			intersection = transform_.TransformLocalIntersectionToWorld(ray, intersection);
		}
	}
	else
	{
		std::vector<Intersection> p1, p2;
		left_->GetPoints(ray, p1, tMin, tMax);
		right_->GetPoints(ray, p2, tMin, tMax);
		std::sort(p1.begin(), p1.end());
		std::sort(p2.begin(), p2.end());
		MergeCSGSegments(ray, p1, p2, points, operation_);
	}	
}

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
		if (leftAt >= 0 && rightAt >= 0 && Equals(leftPoints[leftAt].distance, rightPoints[rightAt].distance))
		{
			// overlapping intersection, take the left one arbitrarily and advance both
			currentSegment.startTime = leftPoints[leftAt].distance;
			currentSegment.startIntersection = leftPoints[leftAt];
			leftInside = !leftInside;
			rightInside = !rightInside;
			leftAt--;
			rightAt--;
		}
		else if (leftAt >= 0 && (rightAt == -1 || leftPoints[leftAt].distance > rightPoints[rightAt].distance))
		{
			currentSegment.startTime = leftPoints[leftAt].distance;
			currentSegment.startIntersection = leftPoints[leftAt];
			leftInside = !leftInside;
			leftAt--;
		}
		else
		{
			currentSegment.startTime = rightPoints[rightAt].distance;
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