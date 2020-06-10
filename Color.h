#pragma once

struct Color
{
	Color();
	Color(double red, double green, double blue);
	void Clamp(double minValue, double maxValue);
	double AveragePower() const;
	double r, g, b;
	Color& operator*=(double rhs) { r *= rhs; g *= rhs; b *= rhs; return *this; }
	Color& operator/=(double rhs) { r /= rhs; g /= rhs; b /= rhs; return *this; }
};

Color operator+ (const Color& lhs, const Color& rhs);
Color& operator+= (Color& lhs, const Color& rhs);
Color operator- (const Color& lhs, const Color& rhs);
Color operator* (const Color& lhs, double rhs);
Color operator* (double lhs, const Color& rhs);
Color operator* (const Color& lhs, const Color& rhs);
Color operator/ (const Color& lhs, double rhs);
Color operator/ (const Color& lhs, const Color& rhs);
bool operator== (const Color& lhs, const Color& rhs);

std::ostream& operator<<(std::ostream& out, const Color& c);
std::istream& operator>>(std::istream& in, Color& c);

