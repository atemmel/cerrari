#include "road.hpp"

using Segment = Road::Segment;
using Segments = Road::Segments;
using Iterator = Segments::iterator;
using Range = std::pair<Iterator, Iterator>;
using Seed = std::random_device::result_type;

Road::Road(unsigned min, unsigned max, Seed seed)
	: _gen(seed), _chunkDist(min, max), _coinflip(0, 1), _hillDist(Constants::Hill::None, Constants::Hill::Steep), 
	_curveDist(-Constants::Curve::Sharp, Constants::Curve::Sharp) 
{
	std::cout << seed << '\n';
}

Segments Road::generate(size_t length) 
{
	Segments segments(length);

	//Initiera
	for(size_t i = 0; i < segments.size(); i++)
	{
		auto & segment = segments[i];
		segment.x = Constants::Curve::None;
		segment.y = Constants::Hill::None;

		//Skjut fram segmentet djupare i förhållande till dess position
		segment.z = Constants::Road::SegmentLength - (i * Constants::Road::SegmentLength);
	}

	size_t index = 0;

	_hillState  = Road::HillState::NoHill;
	_curveState = Road::CurveState::NoCurve; 

	_currentHill = Constants::Hill::None;
	_currentCurve = Constants::Curve::None;

	while(index < segments.size() )
	{
		unsigned step = randomChunkLength();
		if(index + step > segments.size() ) step = segments.size() - index;

		auto begin = segments.begin() + index;
		auto end = segments.begin() + index + step;

		Range range(begin, end);

		changeRoad(segments, range);

		_hillState  = static_cast<Road::HillState>(nextHillState(_hillState) );
		_curveState = static_cast<Road::CurveState>(nextCurveState(_curveState) );

		index += step;
	}

	return segments;
}

void Road::changeRoad(const Segments & segments, Range range)
{
	using namespace Constants;

	Segment previous = segments.begin() == range.first 
		? *range.first
		: *(range.first - 1);

	float hill = randomHill();
	float curve = randomCurve();
	
	switch(_hillState)
	{
		case HillState::EnteringHill:
			Utils::transitionHill(range.first, range.second, Hill::None, hill);
			_currentHill = hill;
			break;

		case HillState::HoldingHill:
			Utils::holdHill(range.first, range.second, _currentHill);
			break;

		case HillState::ExitingHill:
			Utils::transitionHill(range.first, range.second, _currentHill, Hill::None);
			_currentHill = Hill::None;
			break;
	}

	switch(_curveState)
	{
		case CurveState::EnteringCurve:
			Utils::enterCurve(range.first, range.second, Curve::None, curve);
			_currentCurve = curve;
			break;
		case CurveState::HoldingCurve:
			Utils::holdCurve(range.first, range.second, _currentCurve);
			break;
		case CurveState::ExitingCurve:
			Utils::exitCurve(range.first, range.second, _currentCurve, Curve::None);
			_currentCurve = Curve::None;
			break;
	}
}

unsigned Road::nextHillState(Road::HillState state)
{
	HillState newState = HillState::NoHill;

	switch(state)
	{
		case HillState::NoHill:
			newState = coinflip()  ? HillState::EnteringHill
										: HillState::NoHill;
			break;
		case HillState::EnteringHill:
			newState = HillState::HoldingHill;
			break;
		case HillState::HoldingHill:
			newState = HillState::ExitingHill;
			break;
		case HillState::ExitingHill:
			newState = HillState::NoHill;
			break;
	}

	return newState;
}

unsigned Road::nextCurveState(CurveState state)
{
	CurveState newState = CurveState::NoCurve;

	switch(state)
	{
		case CurveState::NoCurve:
			newState = coinflip()  ? CurveState::EnteringCurve
										: CurveState::NoCurve;
			break;
		case CurveState::EnteringCurve:
			newState = CurveState::HoldingCurve;
			break;
		case CurveState::HoldingCurve:
			newState = CurveState::ExitingCurve;
			break;
		case CurveState::ExitingCurve:
			newState = CurveState::NoCurve;
			break;
	}

	return newState;
}

unsigned Road::randomChunkLength()
{
	return _chunkDist(_gen);
}

bool Road::coinflip()
{
	return _coinflip(_gen);
}

float Road::randomHill()
{
	return _hillDist(_gen);
}

float Road::randomCurve()
{
	return _curveDist(_gen);
}
