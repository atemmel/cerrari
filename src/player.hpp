#ifndef PLAYER_H
#define PLAYER_H

#include "math.hpp"
#include "constants.hpp"

#include "SFML/Graphics.hpp"

/**
 *	@brief Structure for maintaining a player.
 *	
 *	The structure is intended to be moderately open, as a lot of its responsibilites are 
 *	to just contain the correct data.
 */
struct Player
{
	/**
	 *	@brief Direction the player can steer in.
	 */
	enum Direction { Left, Right};

	/**
	 *	@brief Updates the player state, changing his velocity, position and acceleration.
	 */
	void update();

	/**
	 *	@brief Accelerates the player.
	 */
	void accelerate();

	/**
	 *	@brief Deaccelerates the player.
	 *	@note This is not the same as forcing him to brake. 
	 *	To brake is a much sharper action, whereas a 
	 *	deacceleration is a much more smoother and natural 
	 *	movement.
	 */
	void deaccelerate();

	/**
	 * @brief Forces the player to brake.
	 * @note See `Player::deaccelerate`
	 */
	void brake();

	/**
	 *	@brief Stops the player.
	 */
	void stop();

	/**
	 * @brief Makes the player turn.
	 * @param direction Direction for the player to turn.
	 */
	void turn(Direction direction);

	sf::Sprite sprite;
	sf::Vector3f position;
	sf::Vector3f velocity;

	/**
	 *	@brief Acceleration is only calculated for the z-axis.
	 */
	float acceleration = 0.f;

	/**
	 *	@brief Maximum forward velocity the player may have.
	 */
	constexpr static float maxVelocity = 500.f;

	/**
	 *	@brief Dimension for the associated texturerect.
	 */
	const sf::Vector2i spriteDim {63, 35};
	/**
	 *	@brief Position for the associated texturerect.
	 */
	const sf::Vector2i spriteNormalPos {144, 12};
	/**
	 *	@brief Left dimensions for the associated texturerect.
	 */
	const sf::Vector2i spriteLeftDim {63,35};
	/**
	 *	@brief Left position for the associated texturerect.
	 */
	const sf::Vector2i spriteLeftPos {0, 12};
private:
	constexpr static float accPerTick = 20.f;
	float calcVelocity = 0.f;
	float turnVelocity = 16.f;
};

#endif
