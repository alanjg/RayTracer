#include "pch.h"
#include "SignedDistanceFunctionObject.h"

ISignedDistanceFunction::~ISignedDistanceFunction()
{
}

double& GridSignedDistanceFunction::get(int i, int j, int k)
{
	return grid_[i * y_ * z_ + j * z_ + k];
}

double GridSignedDistanceFunction::get(int i, int j, int k) const
{
	return grid_[i * y_ * z_ + j * z_ + k];
}

void GridSignedDistanceFunction::init()
{
	for (int i = 0; i < x_; i++)
	{
		for (int j = 0; j < y_; j++)
		{
			for (int k = 0; k < z_; k++)
			{
				int ii = -1, jj = -1, kk = -1;
				bool outer = false;
				if (i == x_ - 1)
				{
					ii = x_ - 2;
					outer = true;
				}
				else
				{
					ii = i;
				}
				if (j == y_ - 1)
				{
					jj = y_ - 2;
					outer = true;
				}
				else
				{
					jj = j;
				}
				if (k == z_ - 1)
				{
					kk = z_ - 2;
					outer = true;
				}
				else
				{
					kk = k;
				}
				if (outer)
				{
					double& cell = get(i, j, k);
					cell = get(ii, jj, kk);
				}
			}
		}
	}
}

GridSignedDistanceFunction::GridSignedDistanceFunction(int x, int y, int z, double* cells)
{
	x_ = x + 1;
	y_ = y + 1;
	z_ = z + 1;
	grid_ = new double[x_ * y_ * z_];
	for (int i = 0; i < x_ - 1; i++)
	{
		for (int j = 0; j < y_ - 1; j++)
		{
			for (int k = 0; k < z_ - 1; k++)
			{
				double& cell = get(i, j, k);
				cell = cells[i * y * z + j * z + k];
			}
		}
	}
	init();
}

GridSignedDistanceFunction::GridSignedDistanceFunction(std::istream& in)
{
	in >> x_ >> y_ >> z_;
	x_++;
	y_++;
	z_++;
	grid_ = new double[x_ * y_ * z_];
	for (int i = 0; i < x_ - 1; i++)
	{
		for (int j = 0; j < y_ - 1; j++)
		{
			for (int k = 0; k < z_ - 1; k++)
			{
				double& cell = get(i, j, k);
				in >> cell;
			}
		}
	}
	init();
}

GridSignedDistanceFunction::~GridSignedDistanceFunction()
{
	delete[] grid_;
}

double GridSignedDistanceFunction::Evaluate(const Vector3& point) const
{
	double ddd = 0;
	double x = point[0];
	double y = point[1];
	double z = point[2];
	if (x < 0)
	{
		x = 0;
		ddd += fabs(point[0]);
	}
	if (x > x_ - 2.0)
	{
		x = x_ - 2.0;
		ddd += fabs(point[0] - x_ + 2);
	}

	if (y < 0)
	{
		y = 0;
		ddd += fabs(point[1]);
	}
	if (y > y_ - 2.0)
	{
		y = y_ - 2.0;
		ddd += fabs(point[1] - y_ + 2);
	}

	if (z < 0)
	{
		z = 0;
		ddd += fabs(point[2]);
	}
	if (z > z_ - 2.0)
	{
		z = z_ - 2.0;
		ddd += fabs(point[2] - z_ + 2);
	}

	double xlerp = x - int(x);
	double ylerp = y - int(y);
	double zlerp = z - int(z);

	int d[] = { 0,1 };
	double samples[2][2][2];

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				samples[i][j][k] = get(int(x + i), int(y + j), int(z + k));

	//lerp in x
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			samples[0][i][j] = (1 - xlerp) * samples[0][i][j] + xlerp * samples[1][i][j];

	//lerp in y
	for (int i = 0; i < 2; i++)
		samples[0][0][i] = (1 - ylerp) * samples[0][0][i] + ylerp * samples[0][1][i];

	//lerp in z
	double final = (1 - zlerp) * samples[0][0][0] + zlerp * samples[0][0][1];

	return final + ddd;
}

Vector3 GridSignedDistanceFunction::GetMin() const
{
	return Vector3(0, 0, 0);
}

Vector3 GridSignedDistanceFunction::GetMax() const
{
	return Vector3(x_, y_, z_);
}

SignedDistanceFunctionShape::SignedDistanceFunctionShape(ISignedDistanceFunction* function) :
	function_(function)
{
	Vector3 min = function->GetMin();
	Vector3 max = function->GetMax();
	std::vector<Vector3> vertices;
	vertices.clear();
	vertices.push_back(Vector3(min[0], min[1], min[2]));
	vertices.push_back(Vector3(max[0], min[1], min[2]));
	vertices.push_back(Vector3(max[0], max[1], min[2]));
	vertices.push_back(Vector3(min[0], max[1], min[2]));
	polygons[0] = new Polygon(vertices);

	vertices.clear();
	vertices.push_back(Vector3(min[0], min[1], min[2]));
	vertices.push_back(Vector3(max[0], min[1], min[2]));
	vertices.push_back(Vector3(max[0], min[1], max[2]));
	vertices.push_back(Vector3(min[0], min[1], max[2]));
	polygons[1] = new Polygon(vertices);

	vertices.clear();
	vertices.push_back(Vector3(min[0], min[1], min[2]));
	vertices.push_back(Vector3(min[0], max[1], min[2]));
	vertices.push_back(Vector3(min[0], max[1], max[2]));
	vertices.push_back(Vector3(min[0], min[1], max[2]));
	polygons[2] = new Polygon(vertices);

	vertices.clear();
	vertices.push_back(Vector3(min[0], min[1], max[2]));
	vertices.push_back(Vector3(max[0], min[1], max[2]));
	vertices.push_back(Vector3(max[0], max[1], max[2]));
	vertices.push_back(Vector3(min[0], max[1], max[2]));
	polygons[3] = new Polygon(vertices);

	vertices.clear();
	vertices.push_back(Vector3(min[0], max[1], min[2]));
	vertices.push_back(Vector3(max[0], max[1], min[2]));
	vertices.push_back(Vector3(max[0], max[1], max[2]));
	vertices.push_back(Vector3(min[0], max[1], max[2]));
	polygons[4] = new Polygon(vertices);

	vertices.clear();
	vertices.push_back(Vector3(max[0], min[1], min[2]));
	vertices.push_back(Vector3(max[0], max[1], min[2]));
	vertices.push_back(Vector3(max[0], max[1], max[2]));
	vertices.push_back(Vector3(max[0], min[1], max[2]));
	polygons[5] = new Polygon(vertices);
}

bool SignedDistanceFunctionShape::GetIntersectionPoints(const Ray& ray, std::vector<Intersection>& intersectionPoints, Intersection& firstIntersection, double tMin, double tMax, bool firstPointOnly) const
{
	// Push ray forward by tMin to avoid intersecting before tMin
	Ray rayCopy(ray);
	if (tMin > 0)
	{
		rayCopy.origin += rayCopy.direction * tMin;
	}
	// Evaluate the signed distance function using ray marching.
	// 1. start the ray on the bounding box of the signed distance function.
	// 2. use ray marching by moving forwards by the absolute value of the signed distance function until within 1 cell
	// 3. Step in cell fractions until the sign changes
	// 4. Binary search the remaining distance
	Ray tempRay(rayCopy);
	Intersection intersect;
	intersect.distance = -1;
	Vector3 vmin = function_->GetMin();
	Vector3 vmax = function_->GetMax();

	// 1. If the ray origin is not within the bounding box, move it to the bounding box.
	if (!contains(vmin, vmax, ray.origin))
	{
		for (int i = 0; i < 6; i++)
		{
			Intersection temp;
			// tMin was already applied.
			if (polygons[i]->Intersect(rayCopy, temp, 0, tMax))
			{
				if (temp.distance < intersect.distance || intersect.distance == -1)
				{
					intersect = temp;
				}
			}
		}

		if (intersect.distance == -1)
		{
			return false;
		}
		else
		{
			tempRay.origin = intersect.point;
		}
	}

	// 2. Use ray marching to get within one cell's distance of the boundary
	const double stepDelta = 0.95;
	double dist = function_->Evaluate(tempRay.origin);
	double absDist = abs(dist);
	while (absDist > 1)
	{
		tempRay.origin += absDist * stepDelta * tempRay.direction;
		double newDist = function_->Evaluate(tempRay.origin);
		dist = newDist;
		absDist = abs(dist);
		// If the marched ray is out of bounds, no intersection is possible
		if (!contains(vmin, vmax, tempRay.origin))
		{
			return false;
		}
	}

	// 3. Step cell by cell
	const double cellStepDelta = 0.3; // go one third of a cell at a time
	double startDist = function_->Evaluate(tempRay.origin);
	Vector3 nextPosition = tempRay.origin + tempRay.direction * cellStepDelta;
	double endDist = function_->Evaluate(nextPosition);

	// Walk until the signs don't match
	while (startDist <= 0 && endDist <= 0 || startDist > 0 && endDist > 0)
	{
		tempRay.origin = nextPosition;
		nextPosition = tempRay.origin + tempRay.direction * cellStepDelta;
		startDist = function_->Evaluate(tempRay.origin);
		endDist = function_->Evaluate(nextPosition);
		if (!contains(vmin, vmax, tempRay.origin))
		{
			// If we walk out of bounds, no intersection
			return false;
		}
	}

	// 4. Binary search to find the exact boundary
	double low = 0;
	double high = cellStepDelta;
	double lowDist = startDist;
	double highDist = endDist;
	double mid = 0;
	int iter = 0;
	while (abs(highDist) > 1e-6 && iter < 30)
	{
		mid = (low + high) / 2;
		double midDist = function_->Evaluate(tempRay.origin + tempRay.direction * mid);
		if (midDist > 0 && highDist > 0 || midDist <= 0 && highDist <= 0)
		{
			high = mid;
			highDist = midDist;
		}
		else
		{
			low = mid;
			lowDist = midDist;
		}
		iter++;
	}
	tempRay.origin += tempRay.direction * mid;

	if (firstPointOnly)
	{
		Vector3 point = tempRay.origin;
		Vector3 normal = Gradient(point);
		normal.Normalize();
		firstIntersection.point = point;
		firstIntersection.normal = normal;
		firstIntersection.distance = (ray.origin - firstIntersection.point).Magnitude();
		if (firstIntersection.normal.Dot(ray.direction) > 0)
		{
			firstIntersection.internalIntersection = true;
			firstIntersection.normal *= -1;
		}
		return true;
	}
	else
	{
		// handle csg?
		return true;
	}
}

SignedDistanceFunctionShape::~SignedDistanceFunctionShape()
{
	for (int i = 0; i < 6; i++)
		delete polygons[i];

	delete function_;
}

Vector3 SignedDistanceFunctionShape::GetMin() const
{
	return function_->GetMin();
}

Vector3 SignedDistanceFunctionShape::GetMax() const
{
	return function_->GetMax();
}

Vector3 SignedDistanceFunctionShape::Gradient(Vector3 position) const
{
	Vector3 dx(1, 0, 0);
	Vector3 dy(0, 1, 0);
	Vector3 dz(0, 0, 1);
	double center = function_->Evaluate(position);
	Vector3 g(0, 0, 0);
	g[0] = (function_->Evaluate(position + dx) - center) / dx[0];
	g[1] = (function_->Evaluate(position + dy) - center) / dy[1];
	g[2] = (function_->Evaluate(position + dz) - center) / dz[2];
	return g;
}