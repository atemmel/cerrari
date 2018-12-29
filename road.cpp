#include "road.hpp"

using Segment = Road::Segment;
using Segments = Road::Segments;

Road::Road(unsigned min, unsigned max, unsigned seed)
	: _gen(seed), _chunkDist(min, max), _hillOptions(0, N_HillState), 
	_curveOptions(N_CurveState), _z(Constants::Road::SegmentLength), _step(-1)
{
}

Segments Road::generate(size_t length) 
{
	using namespace Constants;

	Segments segments(length);

	//Initiera
	for(size_t i = 0; i < segments.size(); i++)
	{
		auto & segment = segments[i];
		segment.x = Constants::Curve::None;
		segment.y = Constants::Road::MaxHeight;

		//Skjut fram segmentet i förhållande till dess position
		segment.z = i * -Constants::Road::SegmentLength + Constants::Road::SegmentLength;
	}

	Utils::transitionHill(segments.begin() + 30, segments.begin() + 50, Hill::None, Hill::Steep);
	Utils::holdHill(segments.begin() + 50, segments.begin() + 70, Hill::Steep);
	Utils::transitionHill(segments.begin() + 70, segments.begin() + 90, Hill::Steep, Hill::None);

	Utils::enterCurve(segments.begin() + 130, segments.begin() + 150, Curve::None, -Curve::Medium);
	Utils::holdCurve(segments.begin() + 150, segments.begin() + 170 , -Curve::Medium);
	Utils::exitCurve(segments.begin() + 170, segments.begin() + 190 , -Curve::Medium, Curve::None);

	Utils::enterCurve(segments.begin() + 200, segments.begin() + 210, Curve::None, Curve::Sharp);
	Utils::holdCurve(segments.begin() + 210, segments.begin() + 290 , Curve::Sharp);
	Utils::exitCurve(segments.begin() + 290, segments.end() , Curve::Sharp, Curve::None);

	Utils::transitionHill(segments.begin() + 200, segments.begin() + 220, Hill::None,  Hill::Steep);
	Utils::transitionHill(segments.begin() + 220, segments.begin() + 240, Hill::Steep, Hill::None);
	Utils::transitionHill(segments.begin() + 240, segments.begin() + 260, Hill::None,  Hill::Steep);
	Utils::transitionHill(segments.begin() + 260, segments.begin() + 280, Hill::Steep, Hill::None);
	Utils::transitionHill(segments.begin() + 280, segments.begin() + 300, Hill::None,  Hill::Steep);
	Utils::transitionHill(segments.begin() + 300, segments.begin() + 320, Hill::Steep, Hill::None);

	return segments;
}

/*
sf::Vector3f operator()()
{
	if(_step < 0) 
	{
		_step = _chunkDist(_gen);
		_hillState = static_cast<HillState>(_hillOptions(_gen) );
	}

	sf::Vector3f vector;

	vector.z = _z;
	vector.y = Road::Hill::None;

	std::cout << _step << '\n';
	_z -= Road::SegmentLength;
	--_step;
	return vector;
}
*/
