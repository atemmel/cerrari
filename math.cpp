#include "math.hpp"

float Math::easeIn(float a, float b, float scale)
{
	return a + (b - a) * (scale * scale);
}

float Math::easeOut(float a, float b, float scale)
{
	const float diff = 1.f - scale;
	return a + (b - a) * (1.f - diff * diff);
}

float Math::easeInOut(float a, float b, float scale)
{
	return a + (b - a) * ( -(cosf(Pi * scale) / 2.f) + 0.5f);
}

float Math::interpolate(float a, float b, float scale)
{
	return a + (b - a) * scale;
}

float Math::getDecimal(float a)
{
	int b = a;
	return a - b;
}

float Math::normalize(float min, float max, float value)
{
	return (value - min) / (max - min);
}
