#include "pch.h"
#include "CppUnitTest.h"
#include "AssertHelper.h"
#include "../Objects/Cylinder.h"
#include "../Objects/CSGObject.h"
#include "../Objects/CSGNode.h"
#include "../Materials/DielectricMaterial.h"
#include "../RayTracer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(CSGCylinderRefractionTest)
	{
	public:

		TEST_METHOD(TestRefraction)
		{
			RayTracer rt;

			Material* glass = new DielectricMaterial(1.5);
			Transform t;
			t.SetTranslate(0, 0, 10);
			Cylinder* c1 = new Cylinder(5, 5, t, glass);
			Cylinder* c2 = new Cylinder(4.7, 2.5, t, glass);
			CSGNode* p1 = new CSGNode(c1);
			CSGNode* p2 = new CSGNode(c2);
			CSGNode* d = new CSGNode(p1, p2, CSGOperationType::Difference);

			t.SetTranslate(0, 0, 0);
			CSGObject* object = new CSGObject(d, t, glass);
			rt.GetScene()->AddObject(object);
			rt.GetScene()->BalanceTree();
			Ray ray;
			ray.origin = Vector3(0, 1, 0);
			ray.direction = Vector3(0, 0, 1);
			Intersection intersection = rt.IntersectWithScene(ray);
			Assert::IsFalse(intersection.internalIntersection);
			AssertAreEqual(Vector3(0, 1, 5), intersection.point, 1e-6);
			AssertAreEqual(Vector3(0, 0, -1), intersection.normal, 1e-6);
			Assert::AreEqual(5, intersection.intersectionTime, 1e-6);

			Assert::IsTrue(intersection.hitObject == object);
			ScatterResult scatterResult;
			bool scattered = intersection.hitObject->GetMaterial()->Scatter(intersection, ScatterType::Transmission, scatterResult);
			Assert::IsTrue(scattered);
			
			AssertAreEqual(Vector3(0, 0, 1), scatterResult.SpecularRay.direction, 1e-6);
			AssertAreEqual(Vector3(0, 1, 5), scatterResult.SpecularRay.origin, 1e-6);

			Intersection intersection2 = rt.IntersectWithScene(scatterResult.SpecularRay);
			AssertAreEqual(Vector3(0, 1, 5.3), intersection2.point, 1e-6);
			AssertAreEqual(Vector3(0, 0, -1), intersection2.normal, 1e-6);
			Assert::IsTrue(intersection2.internalIntersection);
			Assert::AreEqual(0.3, intersection2.intersectionTime, 1e-6);

			Assert::IsTrue(intersection2.hitObject == object);

			ScatterResult scatterResult2;
			bool scattered2 = intersection2.hitObject->GetMaterial()->Scatter(intersection2, ScatterType::Transmission, scatterResult2);

			Assert::IsTrue(scattered2);

			AssertAreEqual(Vector3(0, 0, 1), scatterResult2.SpecularRay.direction, 1e-6);
			AssertAreEqual(Vector3(0, 1, 5.3), scatterResult2.SpecularRay.origin, 1e-6);

			Intersection intersection3 = rt.IntersectWithScene(scatterResult2.SpecularRay);
			AssertAreEqual(Vector3(0, 1, 14.7), intersection3.point, 1e-6);
			AssertAreEqual(Vector3(0, 0, -1), intersection3.normal, 1e-6);
			Assert::IsFalse(intersection3.internalIntersection);
			Assert::AreEqual(14.7 - 5.3, intersection3.intersectionTime, 1e-6);

			Assert::IsTrue(intersection3.hitObject == object);
			ScatterResult scatterResult3;
			bool scattered3 = intersection3.hitObject->GetMaterial()->Scatter(intersection3, ScatterType::Transmission, scatterResult3);

			Assert::IsTrue(scattered3);

			AssertAreEqual(Vector3(0, 0, 1), scatterResult3.SpecularRay.direction, 1e-6);
			AssertAreEqual(Vector3(0, 1, 14.7), scatterResult3.SpecularRay.origin, 1e-6);

			Intersection intersection4 = rt.IntersectWithScene(scatterResult3.SpecularRay);
			AssertAreEqual(Vector3(0, 1, 15), intersection4.point, 1e-6);
			AssertAreEqual(Vector3(0, 0, -1), intersection4.normal, 1e-6);
			Assert::IsTrue(intersection4.internalIntersection);
			Assert::AreEqual(0.3, intersection4.intersectionTime, 1e-6);

			Assert::IsTrue(intersection4.hitObject == object);

			ScatterResult scatterResult4;
			bool scattered4 = intersection4.hitObject->GetMaterial()->Scatter(intersection4, ScatterType::Transmission, scatterResult4);

			Assert::IsTrue(scattered4);

			AssertAreEqual(Vector3(0, 0, 1), scatterResult4.SpecularRay.direction, 1e-6);
			AssertAreEqual(Vector3(0, 1, 15), scatterResult4.SpecularRay.origin, 1e-6);
		}
	};
}