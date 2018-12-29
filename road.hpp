#ifndef ROAD_HPP
#define ROAD_HPP

#include "utils.hpp"

#include <random>
#include <iostream>

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

class Road
{
public:
	using Distribution = std::uniform_int_distribution<unsigned>;
	using Segment = sf::Vector3f;
	using Segments = std::vector<Segment>;

	Road(unsigned min, unsigned max, unsigned int seed);

	Segments generate(size_t length);

	void seed(unsigned newSeed);
	unsigned getSeed() const;


private:
	enum HillState { EnteringHill, HoldingHill, ExitingHill, N_HillState };
	enum CurveState { EnteringCurve, HoldingCurve, ExitingCurve, N_CurveState };

	std::mt19937 _gen;
	
	Distribution _chunkDist;
	Distribution _hillOptions;
	Distribution _curveOptions;

	float _z;
	int _step;
	HillState _hillState;
	CurveState _curveState;
};

#endif
