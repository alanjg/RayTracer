#include "pch.h"
#include "CppUnitTest.h"
#include "../KDTree.h"
#include "../Photon.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(ColorTest)
	{
	public:

		TEST_METHOD(TestClamp)
		{
			Color c(2, 2, 2);
			c.Clamp(0, 1);
			Assert::AreEqual(c.r, 1.0);
			Assert::AreEqual(c.g, 1.0);
			Assert::AreEqual(c.b, 1.0);

			Color c2(0.5, 2, -1);
			c2.Clamp(0, 1);
			Assert::AreEqual(c2.r, 0.5);
			Assert::AreEqual(c2.g, 1.0);
			Assert::AreEqual(c2.b, 0.0);
		}
	};
}
