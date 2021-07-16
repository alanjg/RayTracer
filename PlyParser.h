#pragma once
#include "Vector.h"

struct PlyTriangleMesh
{
	std::vector<Vector3> vertices;
	std::vector<int> indices;
};

bool ParseFromPly(const std::string& plyFilePath, PlyTriangleMesh& mesh);