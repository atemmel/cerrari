#include "utils.hpp"

void Utils::moveQuad(sf::VertexArray & arr, sf::Vector2f near, float nearWidth, 
		sf::Vector2f far, float farWidth, sf::Color color)
{
	const sf::Vector2f halfNearWidth	= {nearWidth	* 0.5f, 0.f};
	const sf::Vector2f halfFarWidth		= {farWidth		* 0.5f, 0.f};

	arr[0].position = sf::Vector2f(near - halfNearWidth);
	arr[1].position = sf::Vector2f(near + halfNearWidth);
	arr[2].position = sf::Vector2f(far + halfFarWidth);
	arr[3].position = sf::Vector2f(far - halfFarWidth);

	for(int i = 0; i < 4; i++) arr[i].color = color;
}
