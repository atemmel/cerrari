#ifndef ROAD_HPP
#define ROAD_HPP

#include "utils.hpp"
#include "constants.hpp"

#include <random>
#include <iostream>
#include <utility>

/**
 *	@brief Class for generating sequences of segments, e.g roads.
 */
class Road
{
public:
	/**
	 *	@brief Integer distribution type.
	 */
	using Distribution = std::uniform_int_distribution<unsigned>;

	/**
	 *	@brief Real distribution type.
	 */
	using RealDistribution = std::uniform_real_distribution<float>;

	/**
	 *	@brief Segment type.
	 */
	using Segment = sf::Vector3f;

	/**
	 *	@brief Type for sequences of segments.
	 */
	using Segments = std::vector<Segment>;

	/**
	 *	@brief Sequence iterator type.
	 */
	using Iterator = Segments::iterator;

	/**
	 *	@brief Range type.
	 */
	using Range	= std::pair<Iterator, Iterator>;

	/**
	 *	@brief Seed type.
	 */
	using Seed = std::random_device::result_type;

	/**
	 *	@brief Parametrizied constructor.
	 *	@param min Minimum subsequence length.
	 *	@param max Maximum subsequence length.
	 *	@param seed Seed to generate sequence from
	 */
	Road(unsigned min, unsigned max, Seed seed);

	/**
	 *	@brief Generates sequence of segments.
	 *	@param length Length of sequence to generate.
	 *	@return A new sequence of segments.
	 */
	Segments generate(size_t length);

	/**
	 * @brief Seeds the class once more.
	 * @param newSeed New seed.
	 */
	void seed(Seed newSeed);

	/**
	 *	@brief Extracts seed value.
	 *	@return Current seed value.
	 */
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
