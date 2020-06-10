#pragma once
#include "RayTracer.h"
#include "Scene.h"

class Test
{
	RayTracer& rt;
	Scene* scene_;
public:
	Test(RayTracer& raytracer) : rt(raytracer), scene_(raytracer.GetScene()) {}
	~Test();

	
	void LoadScene1();
	void LoadScene2();
	void LoadScene3();
	void LoadScene4();
	void LoadScene5();
	void LoadScene6();
	void LoadScene7();
	void LoadScene8();

	void LoadScene9();
	void LoadScene10();
	void LoadScene11();

	void LoadTestScene1();
	void LoadTestScene2();
	void LoadTestScene3();
	void LoadTestScene4();
	void LoadTestScene5();
	void LoadTestScene6();
	void LoadTestScene7();
	void LoadTestScene8();
	void LoadTestScene9();
};