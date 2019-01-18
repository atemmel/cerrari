#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/System/Vector2.hpp>

/**
 *	@brief Namespace to try and keep constants inside.
 */
namespace Constants
{

	/**
	 *	@brief Namespace for road-related constants.
	 */
	namespace Road
	{
		constexpr float Width = 2000.f;
		constexpr float MaxHeight = 1000.f;
		constexpr float SegmentLength = 1600.f;
	};

	/**
	 *	@brief Namespace for Curve-related constants.
	 */
	namespace Curve
	{
		constexpr float None = 0.f;
		constexpr float Weak = 5.f;
		constexpr float Medium = 10.f;
		constexpr float Sharp = 20.f;
	};

	/**
	 *	@brief Namespace for hill-related constants.
	 */
	namespace Hill
	{
		constexpr float None = Road::MaxHeight;
		constexpr float Weak = (Road::MaxHeight + 1000.f);
		constexpr float Medium = (Road::MaxHeight + 2000.f);
		constexpr float Steep = (Road::MaxHeight + 4000.f);
	};

	/**
	 *	@brief Default window size.
	 */
	const sf::Vector2f window = {1280.f, 720.f};
};


#endif
