#pragma once
#include "Intersection.h"
#include "IntersectionAccelerator.h"
#include "Object.h"
#include "Ray.h"
#include "Vector.h"

class OctNode
{
protected:
	const std::vector<std::unique_ptr<IObject>>& objects_;
	Vector3 vmin_, vmax_;
public:
	OctNode(const Vector3& vmin, const Vector3& vmax, const std::vector<std::unique_ptr<IObject>>& objects);
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) = 0;
	virtual ~OctNode();
	virtual void PrintTree(int depth) = 0;
};

class OctLeafNode : public OctNode
{
protected:
	std::list<int> objectIds_;
public:
	OctLeafNode(const Vector3& vmin, const Vector3& vmax, const std::list<int>& objectIds, const std::vector<std::unique_ptr<IObject>>& objects);
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax);
	~OctLeafNode();
	virtual void PrintTree(int depth);
};

class OctInternalNode : public OctNode
{
protected:
	Vector3 center_;
	OctNode* children_[8];
public:
	OctInternalNode(const Vector3& vmin, const Vector3& vmax, const std::list<int>& objectIds, const std::vector<std::unique_ptr<IObject>>& objects);
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax);
	~OctInternalNode();
	virtual void PrintTree(int depth);
};
OctNode* Split(const Vector3& vmin, const Vector3& vmax, const std::list<int>& objectIds, const std::vector<std::unique_ptr<IObject>>& objects);

class OctTree : IntersectionAccelerator
{
public:
	OctTree(std::vector<std::unique_ptr<IObject>>& objects);
	void Balance();
	IObject* Traverse(const Ray& ray, Intersection& intersection, double tMin, double tMax) const;
	virtual bool Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const;
	void PrintTree();
private:
	std::vector<std::unique_ptr<IObject>>& objects_;
	OctNode* root_;
};

