#ifndef UTILS_HPP
#define UTILS_HPP

#include "math.hpp"
#include "constants.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <algorithm>

/**
 *	@brief Namespace for general utility functions
 */
namespace Utils
{

/**
 *	@brief Function to modify a range into entering a curve
 *	@param first Start of range.
 *	@param last End of range.
 *	@param enter Starting value.
 *	@param exit Ending value.
 */
template<typename ForwardIterator>
void enterCurve(ForwardIterator first, ForwardIterator last, float enter, float exit)
{
	int max = static_cast<int>(std::distance(first, last) );

	for(int i = 0; i != max; i++)
	{
		float scale = Math::normalize(0.f, max, i);
		first->x = Math::easeIn(enter, exit, scale);
		++first;
	}
}

/**
 *	@brief Function to modify a range into holding a curve
 *	@param first Start of range.
 *	@param last End of range.
 *	@param hold Value to hold.
 */
template<typename ForwardIterator>
void holdCurve(ForwardIterator first, ForwardIterator last, float hold)
{
	std::for_each(first, last, [&](sf::Vector3f & vector)
	{
		vector.x = hold;
	});
}

/**
 *	@brief Function to modify a range into exiting a curve
 *	@param first Start of range.
 *	@param last End of range.
 *	@param enter Starting value.
 *	@param exit Ending value.
 */
template<typename ForwardIterator>
void exitCurve(ForwardIterator first, ForwardIterator last, float enter, float exit)
{
	int max = static_cast<int>(std::distance(first, last) );

	for(int i = 0; i != max; i++)
	{
		float scale = Math::normalize(0.f, max, i);
		first->x = Math::easeOut(enter, exit, scale);
		++first;
	}
}

/**
 *	@brief Function to modify a range into transitioning from/into a hill.
 *	@param first Start of range.
 *	@param last End of range.
 *	@param enter Starting value.
 *	@param exit Ending value.
 */
template<typename ForwardIterator>
void transitionHill(ForwardIterator first, ForwardIterator last, float enter, float exit)
{
	int max = static_cast<int>(std::distance(first, last) );

	for(int i = 0; i != max; i++)
	{
		float scale = Math::normalize(0.f, max, i);
		first->y = Math::easeInOut(enter, exit, scale);
		++first;
	}
}

/**
 *	@brief Function to modify a range into holding a hill
 *	@param first Start of range.
 *	@param last End of range.
 *	@param hold Value to hold. .
 */
template<typename ForwardIterator>
void holdHill(ForwardIterator first, ForwardIterator last, float hold)
{
	std::for_each(first, last, [&](sf::Vector3f & vector)
	{
		vector.y = hold;
	});
}

/**
 *	@brief Function to move a series of quads.
 *	@param arr Array of vertices to modify.
 *	@param near Lower segment..
 *	@param nearWidth Width of lower segment.
 *	@param far Upper segment..
 *	@param farWidth Width of upper segment.
 *	@param roadColor Color of the road.
 *	@param offroadColor Color of the area next to the road.
 */
void moveQuad(sf::VertexArray & arr, sf::Vector2f near, float nearWidth, 
		sf::Vector2f far, float farWidth, sf::Color roadColor, sf::Color offroadColor);

};

#endif
