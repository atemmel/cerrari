#ifndef PLAYER_H
#define PLAYER_H

#include "math.hpp"
#include "constants.hpp"

#include "SFML/Graphics.hpp"

struct Player
{
	enum Direction { Left, Right};

	void update();
	void accelerate();

	void deaccelerate();

	void brake();

	void turn(Direction direction);

	sf::Sprite sprite;
	sf::Vector3f position;
	sf::Vector3f velocity;
	float acceleration = 0.f;

	constexpr static float maxVelocity = 500.f;

	const sf::Vector2i spriteDim {63, 35};
	const sf::Vector2i spriteNormalPos {144, 12};
	const sf::Vector2i spriteLeftDim {63,35};
	const sf::Vector2i spriteLeftPos {0, 12};
private:
	constexpr static float accPerTick = 20.f;
	float calcVelocity = 0.f;
	float turnVelocity = 16.f;
};

#endif
