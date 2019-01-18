#ifndef MATH_HPP
#define MATH_HPP

#include <SFML/System/Vector3.hpp>
#include <cmath>

namespace Math
{

constexpr float Pi = 3.14159265f;
constexpr float HalfPi = Pi / 2.f;

float easeIn(float a, float b, float scale);

float easeOut(float a, float b, float scale);

float easeInOut(float a, float b, float scale);

float interpolate(float a, float b, float scale);

float getDecimal(float a);

float normalize(float min, float max, float value);


};

#endif
