#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants
{

	namespace Road
	{
		constexpr float Width = 2000.f;
		constexpr float MaxHeight = 1000.f;
		constexpr float SegmentLength = 1600.f;
	};

	namespace Curve
	{
		constexpr float None = 0.f;
		constexpr float Weak = 5.f;
		constexpr float Medium = 10.f;
		constexpr float Sharp = 20.f;
	};

	namespace Hill
	{
		constexpr float None = Road::MaxHeight;
		constexpr float Weak = (Road::MaxHeight + 1000.f);
		constexpr float Medium = (Road::MaxHeight + 2000.f);
		constexpr float Steep = (Road::MaxHeight + 4000.f);
	};

};


#endif
