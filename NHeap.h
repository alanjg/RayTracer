#pragma once

// This is a max heap when comparer is less.  Top/Pop will be the largest element.
// This is a min heap when comparer is greater.  Top/Pop will be the smallest element.
// When the heap is at the limit size, Push will only push when it is smaller(max heap) or bigger(min heap) than the current smallest/biggest item.
template<class T, class Comparer = std::less<T> >
class NHeap
{
	int size_;
	int limit_;
	Comparer comparer_;
	std::vector<T> storage_;
	void Initialize(int limit, const Comparer& comparer)
	{
		comparer_ = comparer;
		limit_ = limit;
		storage_.resize(limit);
		size_ = 0;
	}

public:
	NHeap()
	{
		Initialize(0, Comparer());
	}

	NHeap(int limit)
	{
		Initialize(limit, Comparer());
	}

	NHeap(int limit, Comparer comparer)
	{
		Initialize(limit, comparer);
	}

	void Resize(int limit)
	{
		while (size_ > limit)
		{
			Pop();
		}

		limit_ = limit;
	}

	// returns true if an item was pushed
	bool Push(const T& item)
	{
		if (size_ == limit_)
		{
			if (!comparer_(storage_[0], item))
			{
				pop_heap(storage_.begin(), storage_.end(), comparer_);
				storage_[limit_ - 1] = item;
				push_heap(storage_.begin(), storage_.end(), comparer_);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			storage_[size_] = item;
			size_++;
			push_heap(storage_.begin(), storage_.begin() + size_, comparer_);
			return true;
		}
	}

	void Pop()
	{
		pop_heap(storage_.begin(), storage_.begin() + size_, comparer_);
		size_--;
	}

	T& Top()
	{
		return storage_[0];
	}

	void Clear()
	{
		size_ = 0;
	}

	int Limit()
	{
		return limit_;
	}

	int Size()
	{
		return size_;
	}

	typename std::vector<T>::iterator begin()
	{
		return storage_.begin();
	}

	typename std::vector<T>::iterator end()
	{
		return storage_.begin() + size_;
	}
};