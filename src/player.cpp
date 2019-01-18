#include "player.hpp"

void Player::update()
{
	calcVelocity += acceleration;
	calcVelocity = std::clamp(calcVelocity, 0.f, maxVelocity);
	velocity.x *= Math::easeOut(0.f, 1.f, calcVelocity / maxVelocity);

	if(fabs(position.x) > (Constants::Road::Width - sprite.getGlobalBounds().width) * 0.5f) calcVelocity *= 0.92f;
	velocity.z = -calcVelocity;

	if(fabs(velocity.z) - 5.f < 0.f) velocity.z = 0.f;
	position += velocity;
	position.x = std::clamp(position.x, -Constants::Road::Width, Constants::Road::Width);

	sf::IntRect rect = sf::IntRect(spriteNormalPos, spriteDim);

	if(velocity.z != 0.f)
	{
		if(velocity.x > 0.f)
		{
			rect = sf::IntRect(spriteLeftPos + sf::Vector2i(spriteLeftDim.x, 0.f), sf::Vector2i(-spriteLeftDim.x, spriteLeftDim.y));
		}
		else if(velocity.x < 0.f)
		{
			rect = sf::IntRect(spriteLeftPos, spriteLeftDim);
		}
		else
		{
			rect = sf::IntRect(spriteNormalPos, spriteDim);
		}
	}

	sprite.setTextureRect(rect);
	velocity.x = 0.f;
}

void Player::accelerate()
{
	acceleration = accPerTick;
}

void Player::deaccelerate()
{
	acceleration = 12 * velocity.z / maxVelocity;
}

void Player::brake()
{
	acceleration = -accPerTick * 0.5f;
}

void Player::turn(Player::Direction direction)
{
	switch (direction)
	{
		case Player::Direction::Left:
			velocity.x -= turnVelocity;
			break;
		case Player::Direction::Right:
			velocity.x += turnVelocity;
			break;
	}
}
