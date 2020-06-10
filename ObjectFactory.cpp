#include "pch.h"
#include "ObjectFactory.h"
#include "Objects/Sphere.h"
#include "Objects/Polygon.h"
#include "Objects/Ellipsoid.h"
#include "Objects/Cylinder.h"
#include "Objects/Cone.h"
#include "Objects/CSGObject.h"
#include "Objects/CSGNode.h"
#include "Objects/SignedDistanceFunctionObject.h"
#include "Materials/Material.h"
#include "Materials/MaterialManager.h"
#include "Transform.h"
#include "Vector.h"

typedef std::map<std::string, Material*> MaterialMap;
MaterialMap materials;

SignedDistanceFunctionObject* MakeLevelSetObject(std::istream& in)
{
	std::cerr << "Making signed distance object" << std::endl;
	std::string s;
	in >> s;
	Vector3 translate;
	Vector3 axis;
	double angle;
	Vector3 scale;
	ISignedDistanceFunction* function = nullptr;
	Transform transform;
	Material* material = nullptr;
	while (s != "}")
	{
		if (s == "translate" || s == "Translate")
		{
			in >> translate;
			transform.SetTranslate(translate);
		}
		else if (s == "rotate" || s == "Rotate")
		{
			in >> axis >> angle;
			transform.SetRotationDeg(axis, angle);
		}
		else if (s == "scale" || s == "Scale")
		{
			in >> scale;
			transform.SetScale(scale);
		}
		else if (s == "filename" || s == "Filename")
		{
			std::string levelSetFilename;
			in >> levelSetFilename;
			std::ifstream fin(levelSetFilename.c_str());
			function = new GridSignedDistanceFunction(fin);
		}
		else if (s == "material" || s == "Material")
		{
			in >> s;
			material = g_MaterialManager.Get(s);
		}
		in >> s;
	}
	return new SignedDistanceFunctionObject(function, transform, material);
}

Sphere* MakeSphere(std::istream& in)
{
	std::cerr << "Making sphere" << std::endl;
	std::string s;
	in >> s;
	Vector3 translate;
	Vector3 axis;
	double angle;
	Vector3 scale;
	double radius = 0;
	Transform transform;
	Material* material = nullptr;
	while (s != "}")
	{
		if (s == "translate" || s == "Translate")
		{
			in >> translate;
			transform.SetTranslate(translate);
		}
		else if (s == "rotate" || s == "Rotate")
		{
			in >> axis >> angle;
			transform.SetRotationDeg(axis, angle);
		}
		else if (s == "scale" || s == "Scale")
		{
			in >> scale;
			transform.SetScale(scale);
		}
		else if (s == "radius" || s == "Radius")
		{
			in >> radius;
		}
		else if (s == "material" || s == "Material")
		{
			in >> s;
			material = g_MaterialManager.Get(s);
		}
		in >> s;
	}
	return new Sphere(radius, transform, material);
}

Ellipsoid* MakeEllipsoid(std::istream& in)
{
	std::cerr << "Making ellipsoid" << std::endl;
	std::string s;
	in >> s;
	Vector3 translate;
	Vector3 axis;
	double angle;
	Vector3 scale;
	Vector3 radii;
	Transform transform;
	Material* material = nullptr;
	while (s != "}")
	{
		if (s == "translate" || s == "Translate")
		{
			in >> translate;
			transform.SetTranslate(translate);
		}
		else if (s == "rotate" || s == "Rotate")
		{
			in >> axis >> angle;
			transform.SetRotationDeg(axis, angle);
		}
		else if (s == "scale" || s == "Scale")
		{
			in >> scale;
			transform.SetScale(scale);
		}
		else if (s == "Radii" || s == "radii")
		{
			in >> radii;
		}
		else if (s == "material" || s == "Material")
		{
			in >> s;
			material = g_MaterialManager.Get(s);
		}
		in >> s;
	}
	return new Ellipsoid(radii, transform, material);
}

Cylinder* MakeCylinder(std::istream& in)
{
	//		cerr<<"Making cylinder"<<endl;
	std::string s;
	in >> s;
	Vector3 translate;
	Vector3 axis;
	double angle;
	Vector3 scale;
	double radius = 0;
	double height = 0;
	Transform transform;
	Material* material = nullptr;
	while (s != "}")
	{
		if (s == "translate" || s == "Translate")
		{
			in >> translate;
			transform.SetTranslate(translate);
		}
		else if (s == "rotate" || s == "Rotate")
		{
			in >> axis >> angle;
			transform.SetRotationDeg(axis, angle);
		}
		else if (s == "scale" || s == "Scale")
		{
			in >> scale;
			transform.SetScale(scale);
		}
		else if (s == "radius" || s == "Radius")
		{
			in >> radius;
		}
		else if (s == "height" || s == "Height")
		{
			in >> height;
		}
		else if (s == "material" || s == "Material")
		{
			in >> s;

			material = g_MaterialManager.Get(s);
		}
		in >> s;
	}
	return new Cylinder(radius, height, transform, material);
}

Cone* MakeCone(std::istream& in)
{
	//		cerr<<"Making cone"<<endl;
	std::string s;
	in >> s;
	Vector3 translate;
	Vector3 axis;
	double angle;
	Vector3 scale;
	double bottomRadius = 0;
	double topRadius = 0;
	double height = 0;
	Transform transform;
	Material* material = nullptr;
	while (s != "}")
	{
		if (s == "translate" || s == "Translate")
		{
			in >> translate;
			transform.SetTranslate(translate);
		}
		else if (s == "rotate" || s == "Rotate")
		{
			in >> axis >> angle;
			transform.SetRotationDeg(axis, angle);
		}
		else if (s == "scale" || s == "Scale")
		{
			in >> scale;
			transform.SetScale(scale);
		}
		else if (s == "topradius" || s == "topRadius" || s == "Topradius" || s == "TopRadius")
		{
			in >> topRadius;
		}
		else if (s == "bottomradius" || s == "bottomRadius" || s == "bottomradius" || s == "bottomRadius")
		{
			in >> bottomRadius;
		}
		else if (s == "height" || s == "Height")
		{
			in >> height;
		}
		else if (s == "material" || s == "Material")
		{
			in >> s;

			material = g_MaterialManager.Get(s);
		}
		in >> s;
	}
	return new Cone(bottomRadius, topRadius, height, transform, material);
}

Polygon* MakePolygon(std::istream& in)
{
	//		cerr<<"Making polygon"<<endl;
	std::string s;
	in >> s;
	Vector3 translate;
	Vector3 axis;
	double angle;
	Vector3 scale;
	std::vector<Vector3> points;
	Transform transform;
	Material* material = nullptr;
	while (s != "}")
	{
		if (s == "translate" || s == "Translate")
		{
			in >> translate;
			transform.SetTranslate(translate);
		}
		else if (s == "rotate" || s == "Rotate")
		{
			in >> axis >> angle;
			transform.SetRotationDeg(axis, angle);
		}
		else if (s == "scale" || s == "Scale")
		{
			in >> scale;
			transform.SetScale(scale);
		}
		else if (s == "points" || s == "Points")
		{
			//get {
			char c;
			in >> c;
			std::getline(in, s, '}');
			std::istringstream ins(s);
			std::copy(std::istream_iterator<Vector3>(ins),
				std::istream_iterator<Vector3>(), std::back_inserter(points));
		}
		else if (s == "material" || s == "Material")
		{
			in >> s;

			material = g_MaterialManager.Get(s);
		}
		in >> s;
	}
	return new Polygon(points, transform, material);
}

CSGNode* MakeTree(const std::string& equation, const std::map<std::string, IObject*>& nodes)
{
	std::istringstream in(equation);
	std::string name;
	std::stack<std::string> names;
	std::stack<char> ops;
	char op;
	while (in >> name >> op)
	{
		names.push(name);
		ops.push(op);
	}
	std::map<std::string, IObject*>::const_iterator it = nodes.find(name);
	if (it == nodes.end())
	{
		std::cerr << "Cannot find " << name << std::endl;
		exit(0);
	}
	CSGNode* root = new CSGNode(it->second);
	while (!ops.empty())
	{
		op = ops.top();
		ops.pop();
		name = names.top();
		names.pop();
		it = nodes.find(name);
		if (it == nodes.end())
		{
			std::cerr << "Cannot find " << name << std::endl;
			exit(0);
		}
		CSGNode* node = new CSGNode(it->second);
		switch (op)
		{
		case '&':
			root = new CSGNode(node, root, CSGOperationType::Intersection);
			break;
		case '|':
			root = new CSGNode(node, root, CSGOperationType::Union);
			break;
		case '-':
			root = new CSGNode(node, root, CSGOperationType::Difference);
			break;
		}
	}
	return root;
}

CSGObject* MakeCSG(std::istream& in)
{
	std::cerr << "Making CSGObject" << std::endl;
	std::string s;
	in >> s;
	Vector3 translate;
	Vector3 axis;
	double angle;
	Vector3 scale;
	CSGNode* root;
	std::map<std::string, IObject*> nodes;
	std::string equation;
	Transform transform;
	Material* material = nullptr;
	while (s != "}")
	{
		if (s == "translate" || s == "Translate")
		{
			in >> translate;
			transform.SetTranslate(translate);
		}
		else if (s == "rotate" || s == "Rotate")
		{
			in >> axis >> angle;
			transform.SetRotationDeg(axis, angle);
		}
		else if (s == "scale" || s == "Scale")
		{
			in >> scale;
			transform.SetScale(scale);
		}
		else if (s == "root" || s == "Root")
		{
			std::getline(in, s, '\"');
			std::getline(in, equation, '\"');
		}
		else if (s == "object" || s == "Object")
		{
			std::string name;
			IObject* o = MakeObject(in);
			in >> name;

			nodes[name] = o;
			std::cerr << "Found object:" << name << std::endl;

		}
		else if (s == "material" || s == "Material")
		{
			in >> s;
			material = g_MaterialManager.Get(s);
		}
		in >> s;
	}
	//parse equation
	root = MakeTree(equation, nodes);
	return new CSGObject(root, transform, material);
}

Object* MakeObject(std::istream& in)
{
	std::string s;
	in >> s;

	if (s == "Sphere" || s == "sphere")
	{
		char c;
		in >> c;
		return MakeSphere(in);
	}
	else if (s == "Cylinder" || s == "cylinder")
	{
		char c;
		in >> c;
		return MakeCylinder(in);
	}
	else if (s == "polygon" || s == "Polygon")
	{
		char c;
		in >> c;
		return MakePolygon(in);
	}
	else if (s == "cone" || s == "Cone")
	{
		char c;
		in >> c;
		return MakeCone(in);
	}
	else if (s == "Ellipsoid" || s == "ellipsoid")
	{
		char c;
		in >> c;
		return MakeEllipsoid(in);
	}
	else if (s == "csg" || s == "Csg" || s == "CSG")
	{
		char c;
		in >> c;
		return MakeCSG(in);
	}
	else if (s == "LevelSetGrid" || s == "levelsetgrid")
	{
		char c;
		in >> c;
		return MakeLevelSetObject(in);
	}
	else
	{
		return 0;
	}
}