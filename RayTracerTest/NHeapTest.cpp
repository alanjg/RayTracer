#include "pch.h"
#include "CppUnitTest.h"
#include "../KDTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RayTracerTest
{
	TEST_CLASS(NHeapTest)
	{
	public:
		
		TEST_METHOD(TestHeapManipulation)
		{
			NHeap<int> heap(3);

			Assert::AreEqual(0, heap.Size());
			Assert::AreEqual(3, heap.Limit());

			heap.Push(3); // 3
			Assert::AreEqual(1, heap.Size());
			Assert::AreEqual(3, heap.Top());

			heap.Push(4); // 3, 4
			Assert::AreEqual(2, heap.Size());
			Assert::AreEqual(4, heap.Top());

			heap.Push(5); // 3, 4, 5
			Assert::AreEqual(3, heap.Size());
			Assert::AreEqual(5, heap.Top());

			heap.Push(6); // 3, 4, 5
			Assert::AreEqual(3, heap.Size());
			Assert::AreEqual(5, heap.Top());

			heap.Push(1); // 1, 3, 4
			Assert::AreEqual(3, heap.Size());
			Assert::AreEqual(4, heap.Top());

			heap.Push(4); // 1, 3, 4
			Assert::AreEqual(3, heap.Size());
			Assert::AreEqual(4, heap.Top());

			heap.Pop(); // 1, 3
			Assert::AreEqual(2, heap.Size());
			Assert::AreEqual(3, heap.Top());

			heap.Pop(); // 1
			Assert::AreEqual(1, heap.Size());
			Assert::AreEqual(1, heap.Top());

			heap.Pop(); // empty
			Assert::AreEqual(0, heap.Size());
	
			heap.Push(-1); // -1
			Assert::AreEqual(1, heap.Size());
			Assert::AreEqual(-1, heap.Top());

			heap.Push(4); // -1, 4
			Assert::AreEqual(2, heap.Size());
			Assert::AreEqual(4, heap.Top());

			heap.Push(18); // -1, 4, 18
			Assert::AreEqual(3, heap.Size());
			Assert::AreEqual(18, heap.Top());

			heap.Pop(); // -1, 4
			Assert::AreEqual(2, heap.Size());
			Assert::AreEqual(4, heap.Top());

			heap.Pop(); // -1
			Assert::AreEqual(1, heap.Size());
			Assert::AreEqual(-1, heap.Top());

			heap.Pop(); // empty
			Assert::AreEqual(0, heap.Size());
		}
	};
}