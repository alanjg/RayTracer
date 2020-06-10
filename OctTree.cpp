#include "pch.h"
#include "OctTree.h"
#include "Options.h"

OctNode::OctNode(const Vector3& vmin, const Vector3& vmax, const std::vector<std::unique_ptr<IObject>>& objects) :
	vmin_(vmin), vmax_(vmax), objects_(objects)
{
}

OctNode::~OctNode()
{

}

OctLeafNode::~OctLeafNode()
{

}

OctInternalNode::~OctInternalNode()
{
	for (int i = 0; i < 8; i++)
		delete children_[i];
}

OctLeafNode::OctLeafNode(const Vector3& vmin, const Vector3& vmax, const std::list<int>& objectIds, const std::vector<std::unique_ptr<IObject>>& objects) :
	OctNode(vmin, vmax, objects)
{
	objectIds_ = objectIds;
}

bool OctLeafNode::Intersect(const Ray& ray, Intersection& bestIntersection, double tMin, double tMax)
{
	if (!RayBoxIntersect(ray, vmin_, vmax_))
		return false;

	//loop through all objects, call intersect
	//see if earliest is in this box
	int bestIndex = -1;

	for (std::list<int>::iterator it = objectIds_.begin(); it != objectIds_.end(); ++it)
	{
		Intersection intersection;
		if (objects_[*it]->Intersect(ray, intersection, tMin, tMax))
		{
			if (bestIndex == -1 || intersection.intersectionTime < bestIntersection.intersectionTime)
			{
				if (contains(vmin_, vmax_, intersection.point, EPSILON))
				{					
					bestIndex = *it;
					bestIntersection = intersection;
					bestIntersection.hitObject = objects_[*it].get();
				}
			}
		}
	}
	return bestIndex != -1;
}

OctInternalNode::OctInternalNode(const Vector3& vmin, const Vector3& vmax, const std::list<int>& objectIds, const std::vector<std::unique_ptr<IObject>>& objects) :
	OctNode(vmin, vmax, objects), children_{nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{
	int x[] = { 0,0,0,0,1,1,1,1 };
	int y[] = { 0,0,1,1,0,0,1,1 };
	int z[] = { 0,1,0,1,0,1,0,1 };

	Vector3 accum(0, 0, 0);
	int total = 0;
	//for(std::list<int>::const_iterator it = objectIds.begin(); it != objectIds.end(); ++it)
	//{
	//	if(contains(vmin,vmax,objects[*it]->GetMin(),objects[*it]->GetMax()))
	//	{
	//		Vector3 omin = VMax(vmin,objects[*it]->GetMin());
	//		Vector3 omax = VMin(vmax,objects[*it]->GetMax());

	//		accum += (omin+omax)/2.0;
	//		total++;
	//	}
	//}

	if (total == 0)
	{
		center_ = (vmin + vmax) / 2.0;
	}
	else
	{
		center_ = accum / total;
	}

	Vector3 values[3];
	values[0] = vmin;
	values[1] = center_;
	values[2] = vmax;
	for (int i = 0; i < 8; i++)
	{
		children_[i] = Split(Vector3(values[x[i]][0], values[y[i]][1], values[z[i]][2]), Vector3(values[x[i] + 1][0], values[y[i] + 1][1], values[z[i] + 1][2]), objectIds, objects);
	}
}

bool OctInternalNode::Intersect(const Ray& ray, Intersection& bestIntersection, double tMin, double tMax)
{
	int x = ray.origin[0] > center_[0] ? 4 : 0;
	int nx = 4 - x;

	int y = ray.origin[1] > center_[1] ? 2 : 0;
	int ny = 2 - y;

	int z = ray.origin[2] > center_[2] ? 1 : 0;
	int nz = 1 - z;

	if (children_[x + y + z]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	if (children_[nx + y + z]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	if (children_[x + ny + z]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	if (children_[x + y + nz]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	if (children_[nx + ny + z]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	if (children_[nx + y + nz]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	if (children_[x + ny + nz]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	if (children_[nx + ny + nz]->Intersect(ray, bestIntersection, tMin, tMax))
		return true;
	return false;
}

void tab(int n)
{
	for (int i = 0; i < n; i++)
		std::cerr << " ";
}

void OctInternalNode::PrintTree(int depth)
{
	tab(depth);
	std::cerr << "Split at :" << center_ << std::endl;
	for (int i = 0; i < 8; i++)
		children_[i]->PrintTree(depth + 1);
}

void OctLeafNode::PrintTree(int depth)
{
	tab(depth);
	std::cerr << "LeafNode:";
	std::copy(objectIds_.begin(), objectIds_.end(), std::ostream_iterator<int>(std::cerr, " "));
	std::cerr << std::endl;
}

OctTree::OctTree(std::vector<std::unique_ptr<IObject>>& objects) :
	objects_(objects), root_(0)
{

}

void OctTree::PrintTree()
{
	root_->PrintTree(0);
}

OctNode* Split(const Vector3& vmin, const Vector3& vmax, const std::list<int>& objectIds, const std::vector<std::unique_ptr<IObject>>& objects)
{
	std::list<int> nodeObjectIds;
	for (std::list<int>::const_iterator it = objectIds.begin(); it != objectIds.end(); ++it)
	{
		if (contains(vmin, vmax, objects[*it]->GetMin(), objects[*it]->GetMax()))
		{
			nodeObjectIds.push_back(*it);
		}
	}
	//	std::cerr<<"vmin:"<<vmin<<" vmax:"<<vmax<<std::endl;
	//	std::cerr<<"objectIds.size() = " << objectIds.size()<< " volume:"<<volume(vmin,vmax)<<std::endl;
	if (nodeObjectIds.size() < global_options.max_objects_per_octtree_node)
	{
		//		std::cerr<<"Leaf node"<<std::endl;
		return new OctLeafNode(vmin, vmax, nodeObjectIds, objects);
	}
	else
	{
		//		std::cerr<<"Internal node"<<std::endl;
		return new OctInternalNode(vmin, vmax, nodeObjectIds, objects);
	}
}

void OctTree::Balance()
{
	assert(objects_.size() > 0);

	delete root_;
	Vector3 vmin, vmax;
	vmin = objects_[0]->GetMin();

	std::list<int> objectIds;
	for (unsigned int i = 0; i < objects_.size(); i++)
		objectIds.push_back(i);

	for (unsigned int i = 1; i < objects_.size(); i++)
	{
		Vector3 m = objects_[i]->GetMin();
		vmin = VMin(vmin, m);
	}

	vmax = objects_[0]->GetMax();
	for (unsigned int i = 1; i < objects_.size(); i++)
	{
		Vector3 m = objects_[i]->GetMax();
		vmax = VMax(vmax, m);
	}

	std::cerr << "min:" << vmin << std::endl << "max:" << vmax << std::endl;
	root_ = Split(vmin, vmax, objectIds, objects_);
	PrintTree();
}

IObject* OctTree::Traverse(const Ray& ray, Intersection& intersection, double tMin, double tMax) const
{
	if (root_->Intersect(ray, intersection, tMin, tMax))
	{
		return intersection.hitObject;
	}
	else
	{
		return nullptr;
	}
}

bool OctTree::Intersect(const Ray& ray, Intersection& intersection, double tMin, double tMax) const
{
	IObject* hitObject = Traverse(ray, intersection, tMin, tMax);
	return hitObject != nullptr;
}