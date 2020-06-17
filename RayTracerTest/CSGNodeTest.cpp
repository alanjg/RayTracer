#include "pch.h"
#include "CppUnitTest.h"
#include "AssertHelper.h"
#include "../Objects/CSGNode.h"
#include "../Objects/Sphere.h"
#include "../Objects/Cylinder.h"
#include "../RayTracer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(CSGNodeTest)
	{
	public:
		TEST_METHOD(TestCSGUnionSphere)
		{
			Transform t;
			t.SetTranslate(0, 0, 10);
			Sphere ss1(5);
			ShapeObject s1(&ss1, t, nullptr);
			t.SetTranslate(0, 0, 15);
			Sphere ss2(5);
			ShapeObject s2(&ss2, t, nullptr);
			std::vector<Intersection> p1, p2;
			Ray ray;
			ray.origin.Set(0, 0, 0);
			ray.direction.Set(0, 0, 1);
			Intersection firstIntersection;
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			std::vector<Intersection> mergedPoints;
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Union);

			Assert::AreEqual(2, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(5.0, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 0, 5), mergedPoints[0].point, 1e-6);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(20.0, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, 0, 20), mergedPoints[1].point, 1e-6);
		}

		TEST_METHOD(TestCSGIntersectionSphere)
		{
			Transform t;
			t.SetTranslate(0, 0, 10);
			Sphere ss1(5);
			ShapeObject s1(&ss1, t, nullptr);
			t.SetTranslate(0, 0, 15);
			Sphere ss2(5);
			ShapeObject s2(&ss2, t, nullptr);
			std::vector<Intersection> p1, p2;
			Ray ray;
			ray.origin.Set(0, 0, 0);
			ray.direction.Set(0, 0, 1);
			Intersection firstIntersection;
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			std::vector<Intersection> mergedPoints;
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Intersection);

			Assert::AreEqual(2, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(10.0, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 0, 10), mergedPoints[0].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[0].normal[2]);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(15.0, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, 0, 15), mergedPoints[1].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[1].normal[2]);
		}

		TEST_METHOD(TestCSGDifferenceSphere)
		{
			Transform t;
			t.SetTranslate(0, 0, 10);
			Sphere ss1(5);
			ShapeObject s1(&ss1, t, nullptr);
			t.SetTranslate(0, 0, 15);
			Sphere ss2(5);
			ShapeObject s2(&ss2, t, nullptr);
			std::vector<Intersection> p1, p2;
			Ray ray;
			ray.origin.Set(0, 0, 0);
			ray.direction.Set(0, 0, 1);
			Intersection firstIntersection;
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			std::vector<Intersection> mergedPoints;
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Difference);

			Assert::AreEqual(2, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(5.0, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 0, 5), mergedPoints[0].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[0].normal[2]);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(10.0, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, 0, 10), mergedPoints[1].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[1].normal[2]);
		}

		TEST_METHOD(TestCSGUnionCylinder)
		{
			Transform t;
			t.SetTranslate(0, -5, 10);
			Cylinder ss1(5, 10);
			ShapeObject s1(&ss1, t, nullptr);
			t.SetTranslate(0, -5, 10);
			Cylinder ss2(4.7, 10);
			ShapeObject s2(&ss2, t, nullptr);
			std::vector<Intersection> p1, p2;
			Ray ray;

			// Shoot a ray through the round side of the cylinder
			ray.origin.Set(0, 0, 0);
			ray.direction.Set(0, 0, 1);
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			std::vector<Intersection> mergedPoints;
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Union);

			Assert::AreEqual(2, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(5.0, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 0, 5), mergedPoints[0].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[0].normal[2]);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(15.0, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, 0, 15), mergedPoints[1].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[1].normal[2]);

			// shoot a ray from the top to the bottom
			p1.clear();
			p2.clear();
			mergedPoints.clear();
			ray.origin.Set(0, 10, 10);
			ray.direction.Set(0, -1, 0);
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Union);

			Assert::AreEqual(2, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(5.0, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 5, 10), mergedPoints[0].point, 1e-6);
			Assert::AreEqual(1.0, mergedPoints[0].normal[1]);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(15.0, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, -5, 10), mergedPoints[1].point, 1e-6);
			Assert::AreEqual(1.0, mergedPoints[1].normal[1]);
		}

		TEST_METHOD(TestCSGIntersectionCylinder)
		{
			Transform t;
			t.SetTranslate(0, -5, 10);
			Cylinder ss1(5, 10);
			ShapeObject s1(&ss1, t, nullptr);
			t.SetTranslate(0, -5, 10);
			Cylinder ss2(4.7, 10);
			ShapeObject s2(&ss2, t, nullptr);
			std::vector<Intersection> p1, p2;
			Ray ray;

			// Shoot a ray through the round side of the cylinder
			ray.origin.Set(0, 0, 0);
			ray.direction.Set(0, 0, 1);
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			std::vector<Intersection> mergedPoints;
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Intersection);

			Assert::AreEqual(2, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(5.3, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 0, 5.3), mergedPoints[0].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[0].normal[2]);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(14.7, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, 0, 14.7), mergedPoints[1].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[1].normal[2]);

			// shoot a ray from the top to the bottom
			ray.origin.Set(0, 10, 10);
			ray.direction.Set(0, -1, 0);
			p1.clear();
			p2.clear();
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			mergedPoints.clear();
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Intersection);

			Assert::AreEqual(2, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(5.0, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 5, 10), mergedPoints[0].point, 1e-6);
			Assert::AreEqual(1.0, mergedPoints[0].normal[1]);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(15.0, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, -5, 10), mergedPoints[1].point, 1e-6);
			Assert::AreEqual(1.0, mergedPoints[1].normal[1]);
		}

		TEST_METHOD(TestCSGDifferenceCylinder)
		{
			Transform t;
			t.SetTranslate(0, -5, 10);
			Cylinder ss1(5, 10);
			ShapeObject s1(&ss1, t, nullptr);
			t.SetTranslate(0, -5, 10);
			Cylinder ss2(4.7, 10);
			ShapeObject s2(&ss2, t, nullptr);
			std::vector<Intersection> p1, p2;
			Ray ray;

			// Shoot a ray through the round side of the cylinder
			ray.origin.Set(0, 0, 0);
			ray.direction.Set(0, 0, 1);
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			std::vector<Intersection> mergedPoints;
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Difference);

			Assert::AreEqual(4, (int)mergedPoints.size());
			Assert::IsFalse(mergedPoints[0].internalIntersection);
			Assert::AreEqual(5.0, mergedPoints[0].distance);
			AssertAreEqual(Vector3(0, 0, 5), mergedPoints[0].point, 1e-6); 
			Assert::AreEqual(-1.0, mergedPoints[0].normal[2]);

			Assert::IsTrue(mergedPoints[1].internalIntersection);
			Assert::AreEqual(5.3, mergedPoints[1].distance);
			AssertAreEqual(Vector3(0, 0, 5.3), mergedPoints[1].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[1].normal[2]);

			Assert::IsFalse(mergedPoints[2].internalIntersection);
			Assert::AreEqual(14.7, mergedPoints[2].distance);
			AssertAreEqual(Vector3(0, 0, 14.7), mergedPoints[2].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[2].normal[2]);

			Assert::IsTrue(mergedPoints[3].internalIntersection);
			Assert::AreEqual(15.0, mergedPoints[3].distance);
			AssertAreEqual(Vector3(0, 0, 15), mergedPoints[3].point, 1e-6);
			Assert::AreEqual(-1.0, mergedPoints[3].normal[2]);


			// shoot a ray from the top to the bottom
			ray.origin.Set(0, 10, 10);
			ray.direction.Set(0, -1, 0);
			p1.clear();
			p2.clear();
			s1.GetPoints(ray, p1, 0, T_MAX);
			s2.GetPoints(ray, p2, 0, T_MAX);
			sort(p1.begin(), p1.end());
			sort(p2.begin(), p2.end());
			mergedPoints.clear();
			MergeCSGSegments(ray, p1, p2, mergedPoints, CSGOperationType::Difference);

			Assert::AreEqual(0, (int)mergedPoints.size());
		}
	};
}