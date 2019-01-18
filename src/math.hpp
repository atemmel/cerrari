#ifndef MATH_HPP
#define MATH_HPP

#include <SFML/System/Vector3.hpp>
#include <cmath>

/**
 *	@brief Namespace for helper functions related to calculations.
 */

namespace Math
{

/**
 *	@brief Pi as a definition.
 */
constexpr float Pi = 3.14159265f;

/**
 *	@brief Easing function.
 *	@param a Lower bound.
 *	@param b Upper bound.
 *	@param scale Scalar between 0 and 1 specifying how far into the easing function we are.
 *	@return An eased value.
 */
float easeIn(float a, float b, float scale);

/**
 *	@brief Easing function.
 *	@param a Lower bound.
 *	@param b Upper bound.
 *	@param scale Scalar between 0 and 1 specifying how far into the easing function we are.
 *	@return An eased value.
 */
float easeOut(float a, float b, float scale);

/**
 *	@brief Easing function.
 *	@param a Lower bound.
 *	@param b Upper bound.
 *	@param scale Scalar between 0 and 1 specifying how far into the easing function we are.
 *	@return An eased value.
 */
float easeInOut(float a, float b, float scale);

/**
 *	@brief Interpolates between an upper and a lower bound..
 *	@param a Lower bound.
 *	@param b Upper bound.
 *	@param scale Scalar between 0 and 1 specifying how far into the interpolation function we are.
 *	@return An interpolated value.
 */
float interpolate(float a, float b, float scale);

/**
 *	@brief Function for extracting the non-integer part of a floating point value.
 *	@param a The value to extract from.
 *	@return The trailing decimals from the parameter.
 */
float getDecimal(float a);

/**
 *	@brief Normalizing function.
 *	@param min Lower bound.
 *	@param max Upper bound.
 *	@param value A value inside our bound.
 *	@return A value between 0 and 1.
 */
float normalize(float min, float max, float value);


};

#endif
