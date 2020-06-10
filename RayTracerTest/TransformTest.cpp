#include "pch.h"
#include "CppUnitTest.h"
#include "AssertHelper.h"
#include "../Transform.h"
#include "../Ray.h"
#include "../Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(TransformTest)
	{
	public:

		TEST_METHOD(TestInverse)
		{
			Transform t1;
			Ray ray;
			ray.origin = Vector3(5, 6, 7);
			ray.direction = Vector3(-2, -1, .5);
			ray.direction.Normalize();

			t1.SetRotationDeg(0, 0, 1, 30);
			t1.SetTranslate(5, -3, 2);
			t1.SetScale(.1, 1, -.3);

			Matrix44 m = t1.GetMatrix();
			Matrix44 mi = m.GetInverse();
			Matrix44 mm = mi.GetInverse();
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					Assert::IsTrue(Equals(m[i][j], mm[i][j]));
				}
			}

			Ray localRay = t1.TransformWorldToLocal(ray);
			Ray worldRay = t1.TransformLocalToWorld(localRay);

			Assert::IsTrue(ray.origin.equals(worldRay.origin, 1e-6));
			Assert::IsTrue(ray.direction.equals(worldRay.direction, 1e-6));
		}
		TEST_METHOD(TestRotationOrder)
		{
			Transform t1;
			t1.SetTranslate(10, 0, 0);
			t1.SetRotationDeg(Vector3(0, 1, 0), 90);
			Vector3 origin(0, 0, 0);
			Vector3 xunit(1, 0, 0);
			Vector3 yunit(0, 1, 0);
			Vector3 zunit(0, 0, 1);
			Vector3 transformedOrigin = t1.TransformLocalPointToWorld(origin);
			Vector3 transformedxunit = t1.TransformLocalPointToWorld(xunit);
			Vector3 transformedyunit = t1.TransformLocalPointToWorld(yunit);
			Vector3 transformedzunit = t1.TransformLocalPointToWorld(zunit);

			AssertAreEqual(Vector3(10, 0, 0), transformedOrigin, 1e-6);
			AssertAreEqual(Vector3(10, 0, -1), transformedxunit, 1e-6);
			AssertAreEqual(Vector3(10, 1, 0), transformedyunit, 1e-6);
			AssertAreEqual(Vector3(11, 0, 0), transformedzunit, 1e-6);
		}

		TEST_METHOD(TestScaleOrder)
		{
			Transform t1;
			t1.SetTranslate(10, 0, 0);
			t1.SetScale(Vector3(10, 10, 10));
			Vector3 origin(0, 0, 0);
			Vector3 xunit(1, 0, 0);
			Vector3 yunit(0, 1, 0);
			Vector3 zunit(0, 0, 1);
			Vector3 transformedOrigin = t1.TransformLocalPointToWorld(origin);
			Vector3 transformedxunit = t1.TransformLocalPointToWorld(xunit);
			Vector3 transformedyunit = t1.TransformLocalPointToWorld(yunit);
			Vector3 transformedzunit = t1.TransformLocalPointToWorld(zunit);

			AssertAreEqual(Vector3(10, 0, 0), transformedOrigin, 1e-6);
			AssertAreEqual(Vector3(20, 0, 0), transformedxunit, 1e-6);
			AssertAreEqual(Vector3(10, 10, 0), transformedyunit, 1e-6);
			AssertAreEqual(Vector3(10, 0, 10), transformedzunit, 1e-6);
		}
	};
}