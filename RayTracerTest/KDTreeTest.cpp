#include "pch.h"
#include "CppUnitTest.h"
#include "../KDTree.h"
#include "../Photon.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(KDTreeTest)
	{
	public:

		TEST_METHOD(TestHeapManipulation)
		{

			std::vector<Photon> photons;
			Photon p;
			p.position = Vector3(10, 0, 0);
			photons.push_back(p);
			p.position = Vector3(20, 0, 0);
			photons.push_back(p);
			p.position = Vector3(30, 0, 0);
			photons.push_back(p);
			p.position = Vector3(40, 0, 0);
			photons.push_back(p);
			KDTree tree(photons);

			Vector3 position(20, 0, 0);
			int maxPhotons = 3;
			double maxDistance = 45;
			std::vector<Photon> results;
			double maxActualDistance = tree.Search(position, maxPhotons, maxDistance * maxDistance, results);

			Assert::AreEqual(3, (int)results.size());
			Assert::AreEqual(10.0, maxActualDistance);

			sort(results.begin(), results.end(), KDTree::PhotonPlaneComparer(0));
			Assert::AreEqual(10.0, results[0].position[0]);
			Assert::AreEqual(20.0, results[1].position[0]);
			Assert::AreEqual(30.0, results[2].position[0]);
		}
	};
}