#ifndef PLAYER_H
#define PLAYER_H

struct Player
{
	void update()
	{
		//if(fabs(velocity.y - 80.f) < 0.f) velocity.y = 0.f;
		if(fabs(velocity.z) - 5.f < 0.f) velocity.z = 0.f;
		position += velocity;

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
	}

	sf::Sprite sprite;
	sf::Vector3f position;
	sf::Vector3f velocity;

	const sf::Vector2i spriteDim {63, 35};
	const sf::Vector2i spriteNormalPos {144, 12};
	const sf::Vector2i spriteLeftDim {63,35};
	const sf::Vector2i spriteLeftPos {0, 12};
};

#endif
