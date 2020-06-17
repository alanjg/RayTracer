#include "pch.h"
#include "Test.h"
#include "RayTracer.h"
#include "Transform.h"
#include "Vector.h"
#include "KDTree.h"
#include "RayTracer.h"
#include "Ray.h"
#include "Options.h"
#include "Objects/Cylinder.h"
#include "Objects/CSGNode.h"
#include "Objects/CSGObject.h"
#include "Objects/Polygon.h"
#include "Texture.h"
#include "Objects/Sphere.h"
#include "Objects/VolumetricConstantMedium.h"
#include "Objects/SignedDistanceFunctionObject.h"
#include "Materials/LambertianMaterial.h"
#include "Materials/MetalMaterial.h"
#include "Materials/DielectricMaterial.h"
#include "Camera.h"
#include "Scene.h"
#include "Light.h"

void print(std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
	while (begin != end)
	{
		std::cout << *begin++ << " ";
	}
	std::cout << std::endl;
}

Test::~Test()
{
}

void GenSpheres(Scene* scene_, int depth, double radius, Vector3 position, Vector3 from)
{
	if (depth == 0) return;
	Material* m;

	if (rand() % 2 == 1)
	{
		m = new LambertianMaterial(Color(0.8, 0.8, 0.8));
	}
	else
	{
		m = new DielectricMaterial(1.3);
	}
	Transform t;
	t.SetTranslate(position);
	Sphere* s = new Sphere(radius);
	IObject* o = new ShapeObject(s, t, m);
	scene_->AddObject(o);

	Vector3 up(1, 1, 0);
	Vector3 down(1, -1, 0);
	Vector3 left(-1, 1, 0);
	Vector3 right(-1, -1, 0);
	Vector3 in(1, 0, -1);
	Vector3 out(1, 0, 1);

	if (depth % 2)
	{
		up[0] += 0.5;
		down[1] += 0.5;
		left[2] += 0.5;
		right[2] += -0.5;
		in[0] += 0.5;
		out[1] += -0.5;
	}

	if (up.Dot(from) <= 0)
		GenSpheres(scene_, depth - 1, radius / 2, position + 1.5 * up * radius, down);

	if (down.Dot(from) <= 0)
		GenSpheres(scene_, depth - 1, radius / 2, position + 1.5 * down * radius, up);

	if (right.Dot(from) <= 0)
		GenSpheres(scene_, depth - 1, radius / 2, position + 1.5 * right * radius, left);

	if (left.Dot(from) <= 0)
		GenSpheres(scene_, depth - 1, radius / 2, position + 1.5 * left * radius, right);

	if (in.Dot(from) <= 0)
		GenSpheres(scene_, depth - 1, radius / 2, position + 1.5 * in * radius, out);

	if (out.Dot(from) <= 0)
		GenSpheres(scene_, depth - 1, radius / 2, position + 1.5 * out * radius, in);
}

void GenSpheres2(Scene* scene_, int depth, double radius, Vector3 position, Vector3 from)
{
	if (depth == 0) return;

	Material* m = new DielectricMaterial(1.3);

	Transform t;
	t.SetTranslate(position);
	Sphere* s = new Sphere(radius);
	IObject* o = new ShapeObject(s, t, m);
	scene_->AddObject(o);

	Vector3 directions[12];
	directions[0] = Vector3(1, 0, 0);
	directions[1] = Vector3(1, 0, -2);
	directions[2] = Vector3(-1, 0, -2);
	directions[3] = Vector3(-1, 0, 0);
	directions[4] = Vector3(-1, 0, 2);
	directions[5] = Vector3(1, 0, 2);
	directions[6] = Vector3(1, 2, 1);
	directions[7] = Vector3(-1, 2, 1);
	directions[8] = Vector3(0, 1, -1);
	directions[9] = Vector3(1, -2, -1);
	directions[10] = Vector3(-1, -2, -1);
	directions[11] = Vector3(0, -1, 1);

	for (int i = 0; i < 12; i++)
	{
		directions[i].Normalize();
		GenSpheres2(scene_, depth - 1, radius / 2, position + 1.5 * directions[i] * radius, -directions[i]);
	}
}

void Test::LoadScene1()
{
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, 0);
	camera.position = Vector3(0, 0, 150);
	camera.up = Vector3(0, 1, 0);

	GenSpheres(rt.GetScene(), 5, 30, Vector3(0, 0, 0), Vector3(0, 0, 0));
	Light* light = new PointLight(Vector3(0, 150, 150), Color(1000000, 1000000, 1000000), Color(100000, 100000, 100000));
	scene_->AddLight(light);
	scene_->BalanceTree();
	scene_->SetBackgroundColor(Color(0.1, 0.1, 0.1));
}

void Test::LoadScene2()
{
	scene_->SetBackgroundColor(Color(.7, .7, .7));
	Camera& camera = rt.GetScene()->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, 0);
	camera.position = Vector3(0, 0, 150);
	camera.up = Vector3(0, 1, 0);

	Material* m = new MetalMaterial(Color(0.8, 0.8, 0.8), 0.0);
	Transform t;
	t.SetTranslate(-10, 0, 0);
	Sphere* s = new Sphere(8);
	IObject* o = new ShapeObject(s, t, m);
	scene_->AddObject(o);

	Material* m2 = new LambertianMaterial(Color(0.8, 0.0, 0.0));
	t.SetTranslate(20, 0, 0);
	o = new ShapeObject(s, t, m2);
	scene_->AddObject(o);

	Light* light = new PointLight(Vector3(0, 150, 150), Color(1000000, 1000000, 1000000), Color(100000, 100000, 100000));
	scene_->AddLight(light);
	scene_->BalanceTree();
}


void Test::LoadScene3()
{
	scene_->SetBackgroundColor(Color(0.078, 0.361, 0.753));

	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, 0);
	camera.position = Vector3(0, 0, 50);
	camera.up = Vector3(0, 1, 0);
	camera.fov = 45;

	Transform t;
	LambertianMaterial* m = new LambertianMaterial(Color(1,1,1));

	Light* light = new PointLight(Vector3(0, 150, 150), Color(1000000, 100000, 1000000), Color(10, 10, 10));
	scene_->AddLight(light);
	scene_->BalanceTree();
}

void Test::LoadScene4()
{
	scene_->SetBackgroundColor(Color(0.078, 0.361, 0.753));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, 0);
	camera.position = Vector3(2.1, 1.3, 1.7);
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 0, 1));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 45;

	Material* g = new LambertianMaterial(Color(0.8, 0.6, 0.264));
	Transform t;
	t.SetTranslate(0, 0, -30);
	Sphere* s1 = new Sphere(29.5);
	ShapeObject* ground = new ShapeObject(s1, t, g);
	scene_->AddObject(ground);

	Material* m = new MetalMaterial(Color(1,1,1), 0.0);
	t.SetTranslate(0, 0, 0);
	Sphere* s2 = new Sphere(0.5);
	ShapeObject* s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	Sphere* s3 = new Sphere(0.166667);

	t.SetTranslate(0.272166, 0.272166, 0.544331);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.643951, 0.172546, 5.23308e-17);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.172546, 0.643951, 5.23308e-17);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.371785, 0.0996195, 0.544331);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.471405, 0.471405, 5.23308e-17);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.643951, -0.172546, 5.23308e-17);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.0996195, -0.371785, 0.544331);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.172546, -0.643951, 5.23308e-17);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.471405, -0.471405, 5.23308e-17);
	s = new ShapeObject(s3, t, m);
	scene_->AddObject(s);

	Light* light = new PointLight(Vector3(4, 3, 2), Color(1000000, 100000, 1000000), Color(30, 30, 30));
	scene_->AddLight(light);

	light = new PointLight(Vector3(1, -4, 4), Color(1000000, 100000, 1000000), Color(30, 30, 30));
	scene_->AddLight(light);

	light = new PointLight(Vector3(-3, 1, 5), Color(1000000, 100000, 1000000), Color(30, 30, 30));
	scene_->AddLight(light);
	scene_->BalanceTree();
}

void Test::LoadScene5()
{
	scene_->SetBackgroundColor(Color(0.078,0.361,0.753));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0,0,0);
	camera.position = Vector3(2.1, 1.3, 1.7);
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0,0,1));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 45;
	Transform t;
	t.SetTranslate(Vector3(0, 0, -30));
	Material* g = new LambertianMaterial(Color(0.8, 0.6, 0.264));
	Sphere* s = new Sphere(29.5);
	ShapeObject* ground = new ShapeObject(s, t, g);
	scene_->AddObject(ground);

	GenSpheres2(scene_,2,0.32,Vector3(0,0,0),Vector3(0,0,0));

	Light *light = new PointLight(Vector3(4, 3, 2), Color(1000000, 100000, 1000000), Color(100, 100, 100));
	scene_->AddLight(light);

	light = new PointLight(Vector3(1, -4, 4), Color(1000000, 100000, 1000000), Color(100, 100, 100));
	scene_->AddLight(light);

	light = new PointLight(Vector3(-3, 1, 5), Color(1000000, 100000, 1000000), Color(100, 100, 100));
	scene_->AddLight(light);
	scene_->BalanceTree();
}

void Test::LoadScene6()
{
	scene_->SetBackgroundColor(Color(0.078, 0.361, 0.753));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, -1);
	camera.position = Vector3(0, 0, 2);
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 45;

	Material* g = new LambertianMaterial(Color(.8, .3, .3));
	Transform t;
	t.SetTranslate(0, 0, -1);
	Sphere* s1 = new Sphere(.5);
	ShapeObject* ground = new ShapeObject(s1, t, g);
	scene_->AddObject(ground);

	Material* m = new LambertianMaterial(Color(.8, .8, 0));
	t.SetTranslate(0, -100.5, -1);
	Sphere* s2 = new Sphere(100);
	ShapeObject* s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	Material* m1 = new MetalMaterial(Color(.8, .6, .2), 0.0);
	t.SetTranslate(1, 0, -1);
	s = new ShapeObject(s1, t, m1);
	scene_->AddObject(s);

	Material* m2 = new MetalMaterial(Color(.8, .8, .8), 0.0);
	t.SetTranslate(-1, 0, -1);
	s = new ShapeObject(s1, t, m2);
	scene_->AddObject(s);
	scene_->BalanceTree();
}

void Test::LoadScene7()
{
	scene_->SetBackgroundColor(Color(0.078, 0.361, 0.753));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, -1);
	camera.position = Vector3(0, 0, 2);
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 45;

	Material* g = new LambertianMaterial(Color(.8, .3, .3));
	Transform t;
	t.SetTranslate(0, 0, -1);
	Sphere* s1 = new Sphere(.5);
	ShapeObject* ground = new ShapeObject(s1, t, g);
	scene_->AddObject(ground);

	Material* m = new LambertianMaterial(Color(.8, .8, 0));
	t.SetTranslate(0, -100.5, -1);
	Sphere* s2 = new Sphere(100);
	ShapeObject* s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	Material* m1 = new MetalMaterial(Color(.8, .6, .2), 0.0);
	t.SetTranslate(1, 0, -1);
	s = new ShapeObject(s1, t, m1);
	scene_->AddObject(s);

	Material* m2 = new DielectricMaterial(1.5);
	t.SetTranslate(-1, 0, -1);
	s = new ShapeObject(s1, t, m2);
	scene_->AddObject(s);
	scene_->BalanceTree();
}

void Test::LoadScene8()
{
	scene_->SetBackgroundColor(Color(0.378, 0.661, 0.853));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, 0);
	camera.position = Vector3(13, 2, 3);
	camera.aperture = 0.1;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 45;

	Material* g = new LambertianMaterial(Color(.4, .2, .1));
	Transform t;
	t.SetTranslate(-4, 1, 0);
	Sphere* ground = new Sphere(1);
	scene_->AddObject(new ShapeObject(ground, t, g));

	Material* m = new LambertianMaterial(new CheckerTexture(new ConstantTexture(Color(.4, .2, .1)), new ConstantTexture(Color(0.9, 0.9, 0.9))));
	t.SetTranslate(0, -1000, 0);
	Sphere* s = new Sphere(1000);
	scene_->AddObject(new ShapeObject(s, t, m));

	Material* m1 = new MetalMaterial(Color(.7, .6, .5), 0.0);
	t.SetTranslate(4, 1, 0);
	s = new Sphere(1);
	scene_->AddObject(new ShapeObject(s, t, m1));

	Material* m2 = new DielectricMaterial(1.5);
	t.SetTranslate(0, 1, 0);
	s = new Sphere(1);
	scene_->AddObject(new ShapeObject(s, t, m2));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double chooseMat = Random::NextReal();
			Vector3 center(a + 0.9 * Random::NextReal(), 0.2, b + 0.9 * Random::NextReal());
			if ((center - Vector3(4, .2, 0)).Magnitude() > 0.9)
			{
				Material* m3;
				if (chooseMat < 0.8)
				{
					m3 = new LambertianMaterial(Color(Random::NextReal() * Random::NextReal(), Random::NextReal() * Random::NextReal(), Random::NextReal() * Random::NextReal()));
				}
				else if (chooseMat < 0.95)
				{
					m3 = new MetalMaterial(Color(0.5 * (1 + Random::NextReal()), 0.5 * (1 + Random::NextReal()), 0.5 * (1 + Random::NextReal())), 0.5 * Random::NextReal());
				}
				else
				{
					m3 = new DielectricMaterial(1.5);
				}
				t.SetTranslate(center);
				s = new Sphere(.2);
				scene_->AddObject(new ShapeObject(s, t, m3));
			}
		}
	}
	scene_->BalanceTree();
}

void Test::LoadScene9()
{
	//cornell box with shiny + glass spheres
	scene_->SetBackgroundColor(Color(0, 0, 0));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 3, 0);
	camera.position = Vector3(0, 3, 48);
	camera.aperture = 0.05;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 54;

	Material* red = new LambertianMaterial(Color(.7, .1, .1));
	Material* green = new LambertianMaterial(Color(.1, .7, .1));
	Material* blue = new LambertianMaterial(Color(.1, .1, .7));
	Material* grey = new LambertianMaterial(Color(.3, .3, .3));
	Material* light = new DiffuseLightMaterial(Color(1,1,1));
	Material* shiny = new MetalMaterial(Color(.8, .8, .8), 0.1);
	Material* glass = new DielectricMaterial(1.5);

	Transform t;
	t.SetTranslate(-9.5, -13, -12);
	Sphere* s1 = new Sphere(7);
	ShapeObject* s = new ShapeObject(s1, t, shiny);
	scene_->AddObject(s);

	t.SetTranslate(9.5, -13, -4);
	s = new ShapeObject(s1, t, glass);
	scene_->AddObject(s);

	Transform t2;
	std::vector<Vector3> pts({ Vector3(-20,20,20), Vector3(-20,-20,20), Vector3(-20,-20,-20), Vector3(-20,20,-20) });
	
	Polygon* pp = new Polygon(pts);
	ShapeObject* p = new ShapeObject(pp, t2, red);
	scene_->AddObject(p);

	pts = std::vector<Vector3>({ Vector3(20,20,20), Vector3(20,20,-20), Vector3(20,-20,-20), Vector3(20,-20,20) });
	pp = new Polygon(pts);
	p = new ShapeObject(pp, t2, blue);
	scene_->AddObject(p);

	pts = std::vector<Vector3>({ Vector3(-20,20,20), Vector3(-20,20,-20), Vector3(20,20,-20), Vector3(20,20,20) });
	pp = new Polygon(pts);
	p = new ShapeObject(pp, t2, green);
	scene_->AddObject(p);

	auto ret = CreateRectangleAreaLight(Vector3(-5, 19.9, 5), Vector3(-5, 19.9, -5), Vector3(5, 19.9, 5), Color(100000, 100000, 100000), Color(100, 100, 100), t2, light);
	scene_->AddLight(std::move(ret.first));
	scene_->AddObject(std::move(ret.second));

	pts = std::vector<Vector3>({ Vector3(-20,-20,20), Vector3(20,-20,20), Vector3(20,-20,-20), Vector3(-20,-20,-20) });
	pp = new Polygon(pts);
	p = new ShapeObject(pp, t2, grey);
	scene_->AddObject(p);

	pts = std::vector<Vector3>({ Vector3(-20,20,-20), Vector3(-20,-20,-20), Vector3(20,-20,-20), Vector3(20,20,-20) });
	pp = new Polygon(pts);
	p = new ShapeObject(pp, t2, grey);
	scene_->AddObject(p);

	scene_->BalanceTree();
}

void Test::LoadScene10()
{
	// cornell box with smoke spheres
	scene_->SetBackgroundColor(Color(0, 0, 0));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 3, 0);
	camera.position = Vector3(0, 3, 48);
	camera.aperture = 0.05;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 54;

	Material* red = new LambertianMaterial(Color(.7, .1, .1));
	Material* green = new LambertianMaterial(Color(.1, .7, .1));
	Material* blue = new LambertianMaterial(Color(.1, .1, .7));
	Material* grey = new LambertianMaterial(Color(.3, .3, .3));
	Material* light = new DiffuseLightMaterial(Color(2, 2, 2));
	Material* shiny = new MetalMaterial(Color(.8, .8, .8), 0.1);
	Material* glass = new DielectricMaterial(1.5);
	Transform t;
	t.SetTranslate(-9.5, -13, -12);
	Sphere* s1 = new Sphere(7);
	scene_->AddObject(new VolumetricConstantMedium(s1, .1, t, new ConstantTexture(Color(1, 1, 1))));

	t.SetTranslate(9.5, -13, -4);
	scene_->AddObject(new VolumetricConstantMedium(s1, .1, t, new ConstantTexture(Color(0, 0, 0))));

	Transform t2;
	std::vector<Vector3> pts({ Vector3(-20,20,20), Vector3(-20,-20,20), Vector3(-20,-20,-20), Vector3(-20,20,-20) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	pts = std::vector<Vector3>({ Vector3(20,20,20), Vector3(20,20,-20), Vector3(20,-20,-20), Vector3(20,-20,20) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, blue);
	scene_->AddObject(p);

	pts = std::vector<Vector3>({ Vector3(-20,20,20), Vector3(-20,20,-20), Vector3(20,20,-20), Vector3(20,20,20) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	auto ret = CreateRectangleAreaLight(Vector3(-10, 19.9, 10), Vector3(-10, 19.9, -10), Vector3(10, 19.9, 10), Color(100000, 100000, 100000), Color(50, 50, 50), t2, light);
	scene_->AddLight(std::move(ret.first));
	scene_->AddObject(std::move(ret.second));

	pts = std::vector<Vector3>({ Vector3(-20,-20,20), Vector3(20,-20,20), Vector3(20,-20,-20), Vector3(-20,-20,-20) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, grey);
	scene_->AddObject(p);

	pts = std::vector<Vector3>({ Vector3(-20,20,-20), Vector3(-20,-20,-20), Vector3(20,-20,-20), Vector3(20,20,-20) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, grey);
	scene_->AddObject(p);

	scene_->BalanceTree();
}

// diffuse spheres on a plane
void Test::LoadTestScene1()
{
	scene_->SetBackgroundColor(Color(0.078, 0.361, 0.753));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(0, 0, 0);
	camera.position = Vector3(2.1, 1.3, 1.7);
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 0, 1));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 45;

	LambertianMaterial* g = new LambertianMaterial(Color(0.8, 0.6, 0.264));
	Transform t;
	t.SetTranslate(0, 0, -30);
	Shape* sphere = new Sphere(29.5);
	std::unique_ptr<IObject> ground(new ShapeObject(sphere, t, g));

	scene_->AddObject(std::move(ground));

	LambertianMaterial* m = new LambertianMaterial(Color(0.4, 0.45, 0.35));
	
	t.SetTranslate(0, 0, 0);
	Sphere* s1 = new Sphere(0.5);
	IObject* s = new ShapeObject(s1, t, m);
	scene_->AddObject(s);


	t.SetTranslate(0.272166, 0.272166, 0.544331);
	Sphere* s2 = new Sphere(0.166667);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.643951, 0.172546, 5.23308e-17);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.172546, 0.643951, 5.23308e-17);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.371785, 0.0996195, 0.544331);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.471405, 0.471405, 5.23308e-17);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.643951, -0.172546, 5.23308e-17);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.0996195, -0.371785, 0.544331);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(-0.172546, -0.643951, 5.23308e-17);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	t.SetTranslate(0.471405, -0.471405, 5.23308e-17);
	s = new ShapeObject(s2, t, m);
	scene_->AddObject(s);

	std::unique_ptr<Light> light(new PointLight(Vector3(4, 3, 2), Color(4, 4, 4), Color(4, 4, 4)));
	scene_->AddLight(std::move(light));

	light.reset(new PointLight(Vector3(1, -4, 4), Color(4, 4, 4), Color(4, 4, 4)));
	scene_->AddLight(std::move(light));

	light.reset(new PointLight(Vector3(-3, 1, 5), Color(4, 4, 4), Color(4, 4, 4)));
	scene_->AddLight(std::move(light));
	scene_->BalanceTree();
}

// cornell box with two diffuse spheres and a point light
void Test::LoadTestScene2()
{	
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 800);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* white = new LambertianMaterial(Color(.73, .73, .73));

	double lightPower = 3000000;
	Light* light = new PointLight(Vector3(270, 350, -287), Color(lightPower, lightPower, lightPower), Color(100000, 100000, 100000));
	scene_->AddLight(light);

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	Transform t;
	t.SetTranslate(365, 80, -165);
	Sphere* s1 = new Sphere(80);
	ShapeObject* s = new ShapeObject(s1, t, white);
	scene_->AddObject(s);

	t.SetTranslate(130, 120, -395);
	s1 = new Sphere(120);
	s = new ShapeObject(s1, t, white);
	scene_->AddObject(s);

	scene_->BalanceTree();
}

// cornell box with a metal sphere and a glass sphere, and a point light.  Used for global photon map test.
void Test::LoadTestScene3()
{
	// cornell box
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 800);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* grey = new LambertianMaterial(Color(.73, .73, .73));
	Material* metal = new MetalMaterial(Color(1, 1, 1), 0);
	Material* dielectric = new DielectricMaterial(1.5);

	double lightPower = 3000000;
	Light* light = new PointLight(Vector3(270, 350, -287), Color(lightPower, lightPower, lightPower), Color(100000, 100000, 100000));
	scene_->AddLight(light);

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, grey);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, grey);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, grey);
	scene_->AddObject(p);

	Transform t;
	t.SetTranslate(365, 80, -165);
	Sphere* ss = new Sphere(80);
	IObject* so = new ShapeObject(ss, t, dielectric);
	scene_->AddObject(so);

	t.SetTranslate(130, 120, -395);
	ss = new Sphere(120);
	so = new ShapeObject(ss, t, metal);
	scene_->AddObject(so);

	scene_->BalanceTree();
}

// cornell box with a glass sphere, and an area light.  Used for caustic test
void Test::LoadTestScene4()
{
	// cornell box
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 700);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* white = new LambertianMaterial(Color(.73, .73, .73));
	Material* metal = new MetalMaterial(Color(1, 1, 1), 0);
	Material* dielectric = new DielectricMaterial(1.5);
	Material* lightMaterial = new DiffuseLightMaterial(Color(1, 1, 1));
	double lightPower = 3000000;

	Transform t1;
	std::pair<std::unique_ptr<Light>, std::unique_ptr<IObject>> lightPair = CreateRectangleAreaLight(Vector3(220, 550, -287), Vector3(220, 550, -437), Vector3(370, 550, -287), Color(lightPower, lightPower, lightPower), Color(10, 10, 10), t1, lightMaterial);
	scene_->AddLight(std::move(lightPair.first));
	scene_->AddObject(std::move(lightPair.second));

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555)  });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	Transform t;
	t.SetTranslate(365, 80, -165);
	Sphere* s1 = new Sphere(80);
	ShapeObject* s = new ShapeObject(s1, t, dielectric);
	scene_->AddObject(s);

	scene_->BalanceTree();
}


// cornell box with a glass sphere, and an area light.  Used for caustic test
void Test::LoadTestScene5()
{
	// cornell box
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 700);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* white = new LambertianMaterial(Color(.73, .73, .73));
	Material* metal = new MetalMaterial(Color(1, 1, 1), 0);
	Material* dielectric = new DielectricMaterial(1.5);
	Material* lightMaterial = new DiffuseLightMaterial(Color(1, 1, 1));
	double lightPower = 3000000;

	Transform t1;
	std::pair<std::unique_ptr<Light>, std::unique_ptr<IObject>> lightPair = CreateRectangleAreaLight(Vector3(220, 550, -287), Vector3(220, 550, -437), Vector3(370, 550, -287), Color(lightPower, lightPower, lightPower), Color(10, 10, 10), t1, lightMaterial);
	scene_->AddLight(std::move(lightPair.first));
	scene_->AddObject(std::move(lightPair.second));

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	Transform t;
	t.SetTranslate(365, 80, -165);
	Sphere* ss = new Sphere(80);
	ShapeObject* s = new ShapeObject(ss, t, dielectric);
	scene_->AddObject(s);

	scene_->BalanceTree();
}


// cornell box with a reflecting sphere, glass sphere, half full water glass
void Test::LoadTestScene6()
{
	// cornell box
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 700);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* white = new LambertianMaterial(Color(.73, .73, .73));
	Material* metal = new MetalMaterial(Color(1, 1, 1), 0);
	Material* glass = new DielectricMaterial(1.5);
	Material* water = new DielectricMaterial(1.33);
	Material* lightMaterial = new DiffuseLightMaterial(Color(1, 1, 1));
	double lightPower = 3000000;

	Transform t1;
	//std::pair<std::unique_ptr<Light>, std::unique_ptr<Object>> lightPair = CreateRectangleAreaLight(Vector3(220, 550, -287), Vector3(220, 550, -437), Vector3(370, 550, -287), Color(lightPower, lightPower, lightPower), Color(10, 10, 10), t1, lightMaterial);
	//scene_->AddLight(std::move(lightPair.first));
	//scene_->AddObject(std::move(lightPair.second));

	Light* light = new PointLight(Vector3(295, 450, -312), Color(lightPower, lightPower, lightPower), Color(100000, 100000, 100000));
	scene_->AddLight(light);

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	Transform t;
	t.SetTranslate(365, 80, -165);

	Sphere* ss = new Sphere(80);
	ShapeObject* s = new ShapeObject(ss, t, glass);
	scene_->AddObject(s);


	t.SetTranslate(130, 120, -395);
	ss = new Sphere(120);
	s = new ShapeObject(ss, t, metal);
	scene_->AddObject(s);

	t.SetTranslate(120, 0, -170);
	Cylinder* co = new Cylinder(40, 90);
	ShapeObject* c = new ShapeObject(co, t, glass);

	scene_->AddObject(c);

	t.SetTranslate(230, 10, -220);
	Cylinder* wo = new Cylinder(37, 90);
	ShapeObject* w = new ShapeObject(wo, t, water);
	//scene_->AddObject(w);

	t.SetTranslate(230, 0, -220);
	Cylinder* c1 = new Cylinder(40, 160);
	Cylinder* c2 = new Cylinder(37, 160);
	CSGNode* p1 = new CSGNode(c1, t);
	CSGNode* p2 = new CSGNode(c2, t);
	CSGNode* d = new CSGNode(p1, p2, CSGOperationType::Difference);

	t.SetTranslate(0, 0, 0);
	CSGShape* csg = new CSGShape(d);
	ShapeObject* object = new ShapeObject(csg, t, glass);
	scene_->AddObject(object);

	scene_->BalanceTree();
}

// cornell box with an empty glass
void Test::LoadTestScene7()
{
	// cornell box
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 700);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* white = new LambertianMaterial(Color(.73, .73, .73));
	Material* metal = new MetalMaterial(Color(1, 1, 1), 0);
	Material* glass = new DielectricMaterial(1.5);
	Material* water = new DielectricMaterial(1.33);
	Material* lightMaterial = new DiffuseLightMaterial(Color(1, 1, 1));
	double lightPower = 3000000;

	Transform t1;

	Light* light = new PointLight(Vector3(295, 450, -312), Color(lightPower, lightPower, lightPower), Color(100000, 100000, 100000));
	scene_->AddLight(light);

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);


	Transform t;
	t.SetTranslate(230, 10, -220);
	Cylinder* ws = new Cylinder(37, 90);
	ShapeObject* w = new ShapeObject(ws, t, water);
	//scene_->AddObject(w);

	t.SetTranslate(225, 0, -225);
	Cylinder* c1 = new Cylinder(220, 160);
	Cylinder* c2 = new Cylinder(215, 160);
	CSGNode* p1 = new CSGNode(c1, t);
	CSGNode* p2 = new CSGNode(c2, t);
	CSGNode* d = new CSGNode(p1, p2, CSGOperationType::Difference);

	t.SetTranslate(0, 0, 0);
	CSGShape* csg = new CSGShape(d);
	ShapeObject* object = new ShapeObject(csg, t, glass);
	scene_->AddObject(object);

	scene_->BalanceTree();
}

// cornell box with a glass with water
void Test::LoadTestScene8()
{
	// cornell box
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 700);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* white = new LambertianMaterial(Color(.73, .73, .73));
	Material* metal = new MetalMaterial(Color(1, 1, 1), 0);
	Material* glass = new DielectricMaterial(1.5);
	Material* water = new DielectricMaterial(1.33);
	Material* lightMaterial = new DiffuseLightMaterial(Color(1, 1, 1));
	double lightPower = 3000000;

	Transform t1;

	Light* light = new PointLight(Vector3(295, 450, -312), Color(lightPower, lightPower, lightPower), Color(100000, 100000, 100000));
	scene_->AddLight(light);

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	Transform t;
	t.SetTranslate(230, 10, -220);
	Cylinder* ws = new Cylinder(37, 90);
	ShapeObject* w = new ShapeObject(ws, t, water);
	scene_->AddObject(w);

	t.SetTranslate(225, 0, -225);
	Cylinder* c1 = new Cylinder(220, 160);
	Cylinder* c2 = new Cylinder(215, 160.1);
	CSGNode* p1 = new CSGNode(c1, t);
	CSGNode* p2 = new CSGNode(c2, t);
	CSGNode* d = new CSGNode(p1, p2, CSGOperationType::Difference);

	t.SetTranslate(0, 0, 0);
	CSGShape* csg = new CSGShape(d);
	ShapeObject* object = new ShapeObject(csg, t, glass);
	scene_->AddObject(object);

	scene_->BalanceTree();
}


// cornell box with a water level set
void Test::LoadTestScene9()
{
	// cornell box
	scene_->SetBackgroundColor(Color(0.05, 0.05, 0.05));
	Camera& camera = scene_->GetCamera();
	camera.centerOfInterest = Vector3(278, 278, 0);
	camera.position = Vector3(278, 278, 700);
	camera.aperture = 0.0;
	Vector3 right = (camera.centerOfInterest - camera.position).Cross(Vector3(0, 1, 0));
	right.Normalize();
	Vector3 up = right.Cross(camera.centerOfInterest - camera.position);
	up.Normalize();
	camera.up = up;
	camera.fov = 40;

	Material* red = new LambertianMaterial(Color(.65, .05, .05));
	Material* green = new LambertianMaterial(Color(.12, .45, .15));
	Material* white = new LambertianMaterial(Color(.73, .73, .73));
	Material* metal = new MetalMaterial(Color(1, 1, 1), 0);
	Material* glass = new DielectricMaterial(1.5);
	Material* water = new DielectricMaterial(1.33);
	Material* lightMaterial = new DiffuseLightMaterial(Color(1, 1, 1));
	double lightPower = 3000000;

	Transform t1;

	Light* light = new PointLight(Vector3(295, 450, -312), Color(lightPower, lightPower, lightPower), Color(100000, 100000, 100000));
	scene_->AddLight(light);

	Transform t2;
	//right wall
	std::vector<Vector3> pts({ Vector3(555,0,0), Vector3(555,555,0), Vector3(555,555,-555), Vector3(555,0,-555) });
	Polygon* ps = new Polygon(pts);
	ShapeObject* p = new ShapeObject(ps, t2, green);
	scene_->AddObject(p);

	//left wall
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(0,0,-555), Vector3(0,555,-555), Vector3(0,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, red);
	scene_->AddObject(p);

	//ceiling
	pts = std::vector<Vector3>({ Vector3(0,555,0), Vector3(0,555,-555), Vector3(555,555,-555),  Vector3(555,555,0) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//floor
	pts = std::vector<Vector3>({ Vector3(0,0,0), Vector3(555,0,0), Vector3(555,0,-555), Vector3(0,0,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	//back wall
	pts = std::vector<Vector3>({ Vector3(0,0,-555), Vector3(555,0,-555), Vector3(555,555,-555), Vector3(0,555,-555) });
	ps = new Polygon(pts);
	p = new ShapeObject(ps, t2, white);
	scene_->AddObject(p);

	Transform t;
	t.SetTranslate(0, 80, -220);
	t.SetScale(8, 8, 8);
	std::ifstream in("g:/code/raytracer/scenes/grid.txt");
	GridSignedDistanceFunction* grid = new GridSignedDistanceFunction(in);
	SignedDistanceFunctionShape* os = new SignedDistanceFunctionShape(grid);
	ShapeObject* o = new ShapeObject(os, t, water);

	int dim = 10;
	double radius = 3;
	Vector3 center = Vector3(dim / 2, dim / 2, dim / 2);
	double* cells = new double[dim * dim * dim];
	int index = 0;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			for (int k = 0; k < dim; k++)
			{
				double dist = (Vector3(i, j, k) - center).Magnitude() - radius;
				cells[index++] = dist;
			}
		}
	}
	t.SetTranslate(0, 80, -220);
	t.SetScale(80, 80, 80);
	//GridSignedDistanceFunction* function = new GridSignedDistanceFunction(dim, dim, dim, cells);
	//SignedDistanceFunctionObject* o = new SignedDistanceFunctionObject(function, t, metal);
	//Sphere* ss = new Sphere(.3);
	//ShapeObject s = new ShapeObject(ss, t, water);
	scene_->AddObject(o);
	//scene_->AddObject(s);

	scene_->BalanceTree();
}