#include "pch.h"
#include "CppUnitTest.h"
#include "../RayTracer.h"
#include "../Scene.h"
#include "../KDTree.h"
#include "../Photon.h"
#include "../Objects/Polygon.h"
#include "../Materials/LambertianMaterial.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(RayPolygonIntersectionTest)
	{
	public:

		TEST_METHOD(TestPolygonIntersection)
		{
			Material* white = new LambertianMaterial(Color(.73, .73, .73));
			Material* lightMaterial = new DiffuseLightMaterial(Color(1, 1, 1));
			double lightPower = 3000000;

			Transform t1;
			std::pair<std::unique_ptr<Light>, std::unique_ptr<Object>> lightPair = CreateRectangleAreaLight(Vector3(220, 550, -287), Vector3(220, 550, -437), Vector3(370, 550, -287), Color(lightPower, lightPower, lightPower), Color(10, 10, 10), t1, lightMaterial);
			Ray ray;
			ray.origin = Vector3(220, 555, 287);
			ray.direction = Vector3(0, -1, 0);
			Intersection intersection;
			bool hit = lightPair.second->Intersect(ray, intersection, 0.001, 10000);
			Assert::IsFalse(hit);
		}
	};
}