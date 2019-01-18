#ifndef ROAD_HPP
#define ROAD_HPP

#include "utils.hpp"
#include "constants.hpp"

#include <random>
#include <iostream>
#include <utility>

class Road
{
public:
	using Distribution = std::uniform_int_distribution<unsigned>;
	using RealDistribution = std::uniform_real_distribution<float>;
	using Segment = sf::Vector3f;
	using Segments = std::vector<Segment>;
	using Iterator = Segments::iterator;
	using Range	= std::pair<Iterator, Iterator>;
	using Seed = std::random_device::result_type;

	Road(unsigned min, unsigned max, Seed seed);

	Segments generate(size_t length);

	void seed(Seed newSeed);
	Seed getSeed() const;


private:
	enum HillState  { NoHill,  EnteringHill,  HoldingHill,  ExitingHill,  N_HillState  };
	enum CurveState { NoCurve, EnteringCurve, HoldingCurve, ExitingCurve, N_CurveState };

	void changeRoad(const Segments & segments, Range range);

	unsigned nextHillState(HillState state);
	unsigned nextCurveState(CurveState state);

	unsigned randomChunkLength();
	bool coinflip();
	float randomHill();
	float randomCurve();

	std::mt19937 _gen;
	
	Distribution _chunkDist;
	Distribution _coinflip;
	RealDistribution _hillDist;
	RealDistribution _curveDist;

	HillState _hillState;
	CurveState _curveState;

	float _currentCurve; 
	float _currentHill;

};

#endif
