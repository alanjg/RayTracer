#include "pch.h"
#include "CppUnitTest.h"
#include "../KDTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(PhotonHeapTest)
	{
	public:

		TEST_METHOD(TestHeapManipulation)
		{
			Vector3 position(10, 0, 0);
			KDTree::PhotonComparer comparer(position);
			NHeap<Photon, KDTree::PhotonComparer> heap(2, comparer);
			Photon p;
			
			p.position = Vector3(15, 0, 0);
			heap.Push(p);
			
			p.position = Vector3(25, 0, 0);
			heap.Push(p);
			
			p.position = Vector3(35, 0, 0);
			heap.Push(p);

			Assert::AreEqual(25.0, heap.Top().position[0]);

			p.position = Vector3(45, 0, 0);
			heap.Push(p);

			Assert::AreEqual(25.0, heap.Top().position[0]);

			p.position = Vector3(12, 0, 0);
			heap.Push(p);

			Assert::AreEqual(15.0, heap.Top().position[0]);

			heap.Pop();
			Assert::AreEqual(12.0, heap.Top().position[0]);
		}
	};
}