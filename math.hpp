#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

namespace Math
{

constexpr float Pi = 3.14159265f;
constexpr float HalfPi = Pi / 2.f;

//using sin = sinf;
//using cos = cosf;
//using tan = tanf;

float easeIn(float a, float b, float scale)
{
	return a + (b - a) * (scale * scale);
}

float easeOut(float a, float b, float scale)
{
	const float diff = 1.f - scale;
	return a + (b - a) * (1.f - diff * diff);
}

float easeInOut(float a, float b, float scale)
{
	return a + (b - a) * ( -(cosf(Pi * scale) / 2.f) + 0.5f);
}

float interpolate(float a, float b, float scale)
{
	return a + (b - a) * scale;
}

float getDecimal(float a)
{
	int b = a;
	return a - b;
}

float normalize(float min, float max, float value)
{
	return (value - min) / (max - min);
}

};

#endif
