#include "pch.h"
#include "CppUnitTest.h"
#include "AssertHelper.h"
#include "../Objects/SignedDistanceFunctionObject.h"
#include "../RayTracer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(SignedDistanceObjectTest)
	{
	public:
		GridSignedDistanceFunction* MakeSphere(int dim, double radius)
		{
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
			GridSignedDistanceFunction* function = new GridSignedDistanceFunction(dim, dim, dim, cells);
			return function;
		}

		TEST_METHOD(TestIntersectPlane)
		{
			//each row is the value for one plane in x.  Function is zero at x=3
			const char* functionDefinition = "5 5 5\
			3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3\
			2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2\
			1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\
			0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\
			-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1\
			";
			RayTracer rt;

			Transform t;
			t.SetTranslate(0, 0, 0);
			std::string f(functionDefinition);
			std::stringstream in(f);
			GridSignedDistanceFunction* function = new GridSignedDistanceFunction(in);
			SignedDistanceFunctionShape* shape = new SignedDistanceFunctionShape(function);
			ShapeObject* object = new ShapeObject(shape, t, nullptr);
			rt.GetScene()->AddObject(object);
			rt.GetScene()->BalanceTree();

			Ray ray;
			ray.origin = Vector3(-5, 2, 2);
			ray.direction = Vector3(1, 0, 0);
			Intersection intersection = rt.IntersectWithScene(ray);
			Assert::IsFalse(intersection.internalIntersection);
			AssertAreEqual(Vector3(3, 2, 2), intersection.point, 1e-6);
			AssertAreEqual(Vector3(-1, 0, 0), intersection.normal, 1e-6);
			Assert::AreEqual(8, intersection.distance, 1e-6);

			Assert::IsTrue(intersection.hitObject == object);
		}

		TEST_METHOD(TestIntersectSphere)
		{
			int dim = 10;
			double radius = 3;
			Vector3 center = Vector3(dim / 2, dim / 2, dim / 2);
			RayTracer rt;

			Transform t;
			t.SetTranslate(0, 0, 0);
			GridSignedDistanceFunction* function = MakeSphere(dim, radius);
			SignedDistanceFunctionShape* shape = new SignedDistanceFunctionShape(function);
			ShapeObject* object = new ShapeObject(shape, t, nullptr);
			rt.GetScene()->AddObject(object);
			rt.GetScene()->BalanceTree();

			Ray ray;
			ray.origin = center - Vector3(10, 0, 0);
			ray.direction = Vector3(1, 0, 0);
			Intersection intersection = rt.IntersectWithScene(ray);
			Assert::IsFalse(intersection.internalIntersection);
			AssertAreEqual(center - Vector3(radius, 0, 0), intersection.point, 1e-6);
			Assert::AreEqual(10 - radius, intersection.distance, 1e-6);

			Assert::IsTrue(intersection.hitObject == object);
		}

		TEST_METHOD(TestIntersectSphereWithTransform)
		{
			int dim = 10;
			double radius = 3;
			Vector3 center = Vector3(dim / 2, dim / 2, dim / 2);
			
			RayTracer rt;

			Transform t;
			t.SetTranslate(10, 20, 30);
			t.SetScale(10, 10, 10);
			GridSignedDistanceFunction* function = MakeSphere(dim, radius);
			SignedDistanceFunctionShape* shape = new SignedDistanceFunctionShape(function);
			ShapeObject* object = new ShapeObject(shape, t, nullptr);
			rt.GetScene()->AddObject(object);
			rt.GetScene()->BalanceTree();

			Ray ray;
			center = t.TransformLocalPointToWorld(center);
			ray.origin = center - Vector3(100, 0, 0);
			ray.direction = Vector3(1, 0, 0);
			Intersection intersection = rt.IntersectWithScene(ray);
			Assert::IsFalse(intersection.internalIntersection);
			Vector3 expectedIntersectionPoint = center - Vector3(radius * 10, 0, 0);
			AssertAreEqual(expectedIntersectionPoint, intersection.point, 1e-5);
			Assert::AreEqual(100 - radius*10, intersection.distance, 1e-5);

			Assert::IsTrue(intersection.hitObject == object);
		}

		TEST_METHOD(TestInternalIntersectSphere)
		{
			int dim = 10;
			double radius = 3;
			Vector3 center = Vector3(dim / 2, dim / 2, dim / 2);
			
			RayTracer rt;

			Transform t;
			t.SetTranslate(0, 0, 0);
			GridSignedDistanceFunction* function = MakeSphere(dim, radius);
			SignedDistanceFunctionShape* shape = new SignedDistanceFunctionShape(function);
			ShapeObject* object = new ShapeObject(shape, t, nullptr);
			rt.GetScene()->AddObject(object);
			rt.GetScene()->BalanceTree();

			Ray ray;
			ray.origin = center - Vector3(10, 0, 0);
			ray.direction = Vector3(1, 0, 0);
			Intersection intersection = rt.IntersectWithScene(ray);
			Assert::IsFalse(intersection.internalIntersection);
			AssertAreEqual(center - Vector3(radius, 0, 0), intersection.point, 1e-6);
			Assert::AreEqual(10 - radius, intersection.distance, 1e-6);

			Assert::IsTrue(intersection.hitObject == object);

			Ray ray2;
			ray2.origin = intersection.point;
			ray2.direction = Vector3(1, 0, 0);
			Intersection intersection2 = rt.IntersectWithScene(ray2);
			Assert::IsTrue(intersection2.internalIntersection);
			AssertAreEqual(center + Vector3(radius, 0, 0), intersection2.point, 1e-6);
			Assert::AreEqual(2 * radius, intersection2.distance, 1e-6);
		}
	};
}