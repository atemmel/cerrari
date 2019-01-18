#ifndef UTILS_HPP
#define UTILS_HPP

#include "math.hpp"
#include "constants.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <algorithm>

namespace Utils
{

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

template<typename ForwardIterator>
void holdCurve(ForwardIterator first, ForwardIterator last, float hold)
{
	std::for_each(first, last, [&](sf::Vector3f & vector)
	{
		vector.x = hold;
	});
}

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

template<typename ForwardIterator>
void holdHill(ForwardIterator first, ForwardIterator last, float hold)
{
	std::for_each(first, last, [&](sf::Vector3f & vector)
	{
		vector.y = hold;
	});
}

void moveQuad(sf::VertexArray & arr, sf::Vector2f near, float nearWidth, 
		sf::Vector2f far, float farWidth, sf::Color roadColor, sf::Color offroadColor);

};

#endif
