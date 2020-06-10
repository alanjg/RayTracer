#include "pch.h"
#include "CppUnitTest.h"
#include "AssertHelper.h"
#include "../Materials/DielectricMaterial.h"
#include "../Intersection.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(DielectricMaterialTest)
	{
	public:

		TEST_METHOD(TestDirectAngleScatter)
		{
			DielectricMaterial m(1.3);
			Intersection intersection;
			intersection.point.Set(0, 0, 0);
			intersection.normal.Set(0, 0, -1);
			intersection.incidentRay.origin.Set(0, 0, -1);
			intersection.incidentRay.direction.Set(0, 0, 1);
			ScatterResult scatterResult1, scatterResult2;
			m.Scatter(intersection, ScatterType::Specular, scatterResult1);
			m.Scatter(intersection, ScatterType::Transmission, scatterResult2);

			Assert::AreEqual(1.0, scatterResult1.resultProbability + scatterResult2.resultProbability, 1e-6);
			AssertAreEqual(Vector3(0, 0, -1), scatterResult1.SpecularRay.direction, 1e-6);
			AssertAreEqual(Vector3(0, 0, 1), scatterResult2.SpecularRay.direction, 1e-6);
		}

		TEST_METHOD(TestDiagonalAngleScatter)
		{
			double ri = 1.3;
			double angle = M_PI / 4;
			DielectricMaterial m(ri);
			Intersection intersection;
			intersection.point.Set(0, 0, 0);
			intersection.normal.Set(0, 0, -1);
			intersection.incidentRay.origin.Set(0, -1, -1);
			intersection.incidentRay.direction.Set(0, 1, 1); // 45 degree angle
			intersection.incidentRay.direction.Normalize();
			Assert::AreEqual(angle, acos(intersection.incidentRay.direction.Dot(-intersection.normal)), 1e-6);

			ScatterResult scatterResult1, scatterResult2;
			m.Scatter(intersection, ScatterType::Specular, scatterResult1);
			m.Scatter(intersection, ScatterType::Transmission, scatterResult2);

			Assert::AreEqual(1.0, scatterResult1.resultProbability + scatterResult2.resultProbability, 1e-6);
			
			double expectedRefractedAngle = asin(sin(angle) / ri);
			double actualRefractedAngle = acos(scatterResult2.SpecularRay.direction.Dot(-intersection.normal));
			Assert::AreEqual(expectedRefractedAngle, actualRefractedAngle, 1e-6);

			double actualReflectedAngle = acos(scatterResult1.SpecularRay.direction.Dot(intersection.normal));
			Assert::AreEqual(angle, actualReflectedAngle, 1e-6);
		}

		TEST_METHOD(TestGlancingAngleScatter)
		{
			double ri = 1.3;
			DielectricMaterial m(ri);
			Intersection intersection;
			intersection.point.Set(0, 0, 0);
			intersection.normal.Set(0, 0, -1);
			intersection.incidentRay.origin.Set(0, -1, -1);
			intersection.incidentRay.direction.Set(0, 10, 1);
			intersection.incidentRay.direction.Normalize();
			double angle = acos(intersection.incidentRay.direction.Dot(-intersection.normal));

			ScatterResult scatterResult1, scatterResult2;
			m.Scatter(intersection, ScatterType::Specular, scatterResult1);
			m.Scatter(intersection, ScatterType::Transmission, scatterResult2);

			Assert::AreEqual(1.0, scatterResult1.resultProbability + scatterResult2.resultProbability, 1e-6);

			double expectedRefractedAngle = asin(sin(angle) / ri);
			double actualRefractedAngle = acos(scatterResult2.SpecularRay.direction.Dot(-intersection.normal));
			Assert::AreEqual(expectedRefractedAngle, actualRefractedAngle, 1e-6);

			double actualReflectedAngle = acos(scatterResult1.SpecularRay.direction.Dot(intersection.normal));
			Assert::AreEqual(angle, actualReflectedAngle, 1e-6);
		}
	};
}