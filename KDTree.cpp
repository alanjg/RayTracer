#include "pch.h"
#include "Options.h"
#include "KDTree.h"

void KDTree::Balance(std::vector<Photon>& photons, int begin, int end, int insertAt)
{
	if (global_options.debugOutputLevel >= DebugOutputLevel::DEBUG_OUTPUT_DIAGNOSTIC)
	{
		std::cout << " Balance at = " << insertAt << " stride = " << (end - begin) << std::endl;
	}

	if (begin >= end)
	{
		return;
	}

	if (begin + 1 == end)
	{
		photonHeap_[insertAt] = photons[begin];
		photonHeap_[insertAt].plane = 0;
		return;
	}

	Vector3 min = photons[begin].position;
	Vector3 max = min;
	for (int i = begin + 1; i < end; i++)
	{
		min = VMin(min, photons[i].position);
		max = VMax(max, photons[i].position);
	}
	int plane = 0;
	if (max[1] - min[1] > max[plane] - min[plane])
	{
		plane = 1;
	}

	if (max[2] - min[2] > max[plane] - min[plane])
	{
		plane = 2;
	}

	// partition at median.
	int median = (begin + end) / 2;
	std::nth_element(photons.begin() + begin, photons.begin() + median, photons.begin() + end, PhotonPlaneComparer(plane));
	
	// store the median at the current insertion location in the heap
	photonHeap_[insertAt] = photons[median];
	photonHeap_[insertAt].plane = plane;
	if (global_options.debugOutputLevel >= DebugOutputLevel::DEBUG_OUTPUT_DIAGNOSTIC)
	{
		std::cout << "added photon at " << photonHeap_[insertAt].position << std::endl;
	}

	// recurse on the smaller and larger half of the partitioned sequence.
	Balance(photons, begin, median, insertAt * 2);
	Balance(photons, median + 1, end, insertAt * 2 + 1);
}

void KDTree::Search(unsigned int at, SearchParams& searchParams)
{
	if (at >= photonHeap_.size())
	{
		return;
	}
	Photon& photon = photonHeap_[at];
	if (photon.plane == 3)
	{
		return;
	}
	
	double distanceSquared = (photon.position - searchParams.position_).MagnitudeSquared();
	if (distanceSquared <= searchParams.maxDistanceSquared_)
	{
		bool added = searchParams.resultsHeap_.Push(photon);
		if (global_options.debugOutputLevel >= DebugOutputLevel::DEBUG_OUTPUT_DIAGNOSTIC)
		{
			std::cout << "Photon map found hit at " << photon.position << " for " << searchParams.position_ << std::endl;
		}

		// if we have maxPhotons_ in results, set maxDistanceSquared_ to the max of results
		if (added && searchParams.resultsHeap_.Size() == searchParams.maxPhotons_)
		{
			searchParams.maxDistanceSquared_ = (searchParams.resultsHeap_.Top().position - searchParams.position_).MagnitudeSquared();
		}
	}

	double distance = photon.position[photon.plane] - searchParams.position_[photon.plane];
	if (distance > 0)
	{
		Search(at * 2, searchParams);
		if (distance * distance <= searchParams.maxDistanceSquared_)
		{
			Search(at * 2 + 1, searchParams);
		}
	}
	else
	{
		Search(at * 2 + 1, searchParams);
		if (distance * distance <= searchParams.maxDistanceSquared_)
		{
			Search(at * 2, searchParams);
		}
	}
}

KDTree::KDTree(const std::vector<Photon>& photons)
{
	int heapSize = photons.size();

	// Round up to next greatest power of 2.
	if ((heapSize & (heapSize - 1)) != 0)
	{
		int highBit = heapSize;
		do
		{
			highBit &= highBit - 1;
		} while ((highBit & (highBit - 1)) != 0);
		heapSize = highBit << 1;
	}

	heapSize++;
	photonHeap_.resize(heapSize);
	for (int i = 0; i < heapSize; i++)
	{
		photonHeap_[i].plane = 3;
	}
	std::vector<Photon> temp(photons.begin(), photons.end());
	if (global_options.debugOutputLevel >= DebugOutputLevel::DEBUG_OUTPUT_DIAGNOSTIC)
	{
		std::cout << "heap size is " << heapSize + 1 << " photons = " << temp.size() << std::endl;
	}
	Balance(temp, 0, temp.size(), 1);

}

double KDTree::Search(const Vector3& position, int maxPhotons, double maxDistanceSquared, std::vector<Photon>& results)
{
	SearchParams searchParams(maxPhotons, PhotonComparer(position), maxDistanceSquared, position, maxPhotons);
	Search(1, searchParams);
	
	results.assign(searchParams.resultsHeap_.begin(), searchParams.resultsHeap_.end());
	return (searchParams.resultsHeap_.Top().position - position).Magnitude();
}