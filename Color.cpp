#include "pch.h"
#include "Color.h"
#include "Math.h"

Color::Color()
{
	r = 0.0;
	g = 0.0;
	b = 0.0;
}
Color::Color(double red, double green, double blue)
{
	r = red;
	g = green;
	b = blue;
}

void Color::Clamp(double minValue, double maxValue)
{
	r = std::max(std::min(r, maxValue), minValue);
	g = std::max(std::min(g, maxValue), minValue);
	b = std::max(std::min(b, maxValue), minValue);
}

double Color::AveragePower() const
{
	return (r + g + b) / 3.0;
}

Color operator+ (const Color& lhs, const Color& rhs)
{
	return Color(lhs.r + rhs.r,
		lhs.g + rhs.g,
		lhs.b + rhs.b);
}

Color& operator+= (Color& lhs, const Color& rhs)
{
	lhs.r += rhs.r;
	lhs.g += rhs.g;
	lhs.b += rhs.b;
	return lhs;
}

Color operator- (const Color& lhs, const Color& rhs)
{
	return Color(lhs.r - rhs.r,
		lhs.g - rhs.g,
		lhs.b - rhs.b);
}

Color operator* (const Color& lhs, double rhs)
{
	return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
}

Color operator* (double lhs, const Color& rhs)
{
	return Color(rhs.r * lhs, rhs.g * lhs, rhs.b * lhs);
}

Color operator* (const Color& lhs, const Color& rhs)
{
	return Color(lhs.r * rhs.r,
		lhs.g * rhs.g,
		lhs.b * rhs.b);
}


Color operator/ (const Color& lhs, double rhs)
{
	return lhs * (1.0 / rhs);
}

Color operator/ (const Color& lhs, const Color& rhs)
{
	return Color(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b);
}

bool operator== (const Color& lhs, const Color& rhs)
{
	return lhs.b == rhs.b && lhs.g == rhs.g && lhs.r == rhs.r;
}

std::ostream& operator<<(std::ostream& out, const Color& c)
{
	return out << c.r << " " << c.g << " " << c.b;
}

std::istream& operator>>(std::istream& in, Color& c)
{
	return in >> c.r >> c.g >> c.b;
}