#pragma once

#include "Options.h"
#include "Photon.h"
#include "NHeap.h"
#include "Vector.h"


class KDTree
{
public:
	struct PhotonPlaneComparer
	{
		int plane_;
		PhotonPlaneComparer(int plane)
		{
			plane_ = plane;
		}
		bool operator()(const Photon& lhs, const Photon& rhs) const
		{
			return lhs.position[plane_] < rhs.position[plane_];
		}
	};

	struct PhotonComparer
	{
		PhotonComparer()
		{
			position_ = Vector3(0, 0, 0);
		}
		PhotonComparer(const Vector3& position)
		{
			position_ = position;
		}
		Vector3 position_;
		bool operator()(const Photon& lhs, const Photon& rhs)
		{
			return (lhs.position - position_).Magnitude() < (rhs.position - position_).Magnitude();
		}
	};

private:
	struct SearchParams
	{
		SearchParams(int size, PhotonComparer comparer, double maxDistanceSquared, Vector3 position, int maxPhotons) :
			resultsHeap_(size, comparer), maxDistanceSquared_(maxDistanceSquared), position_(position), maxPhotons_(maxPhotons)
		{
		}

		NHeap<Photon, PhotonComparer> resultsHeap_;
		double maxDistanceSquared_;
		Vector3 position_;
		int maxPhotons_;
	};
	std::vector<Photon> photonHeap_;
	void Balance(std::vector<Photon>& photons, int begin, int end, int insertAt);

	void Search(unsigned int at, SearchParams& searchParams);
public:
	KDTree(const std::vector<Photon>& photons);

	// returns the distance to the furthest photon in results
	double Search(const Vector3& position, int maxPhotons, double maxDistanceSquared, std::vector<Photon>& results);
};

