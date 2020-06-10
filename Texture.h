#pragma once
#include "Color.h"
#include "Vector.h"

class Texture
{
public:
	virtual Color Value(double u, double v, const Vector3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	ConstantTexture(const Color& color) : c(color) {}
	virtual Color Value(double u, double v, const Vector3& p) const
	{
		return c;
	}
private:
	Color c;
};

class CheckerTexture : public Texture
{
private:
	Texture* even, * odd;
public:
	CheckerTexture(Texture* t1, Texture* t2) : even(t1), odd(t2) {}
	virtual Color Value(double u, double v, const Vector3& p) const
	{
		double sines = std::sin(10 * p[0]) * std::sin(10 * p[1]) * std::sin(10 * p[2]);
		if (sines < 0)
		{
			return odd->Value(u, v, p);
		}
		else
		{
			return even->Value(u, v, p);
		}
	}
};
