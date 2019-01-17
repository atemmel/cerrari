#include "utils.hpp"

void Utils::moveQuad(sf::VertexArray & arr, sf::Vector2f near, float nearWidth, 
		sf::Vector2f far, float farWidth, sf::Color roadColor, sf::Color offroadColor)
{
	const sf::Vector2f halfNearWidth	= {nearWidth	* 0.5f, 0.f};
	const sf::Vector2f halfFarWidth		= {farWidth		* 0.5f, 0.f};

	arr[0].position = sf::Vector2f(0.f, near.y);
	arr[1].position = sf::Vector2f(near.x, near.y);
	arr[2].position = sf::Vector2f(near.x, far.y);
	arr[3].position = sf::Vector2f(0.f, far.y);

	arr[4].position = sf::Vector2f(near - halfNearWidth);
	arr[5].position = sf::Vector2f(near + halfNearWidth);
	arr[6].position = sf::Vector2f(far + halfFarWidth);
	arr[7].position = sf::Vector2f(far - halfFarWidth);

	arr[8].position = sf::Vector2f(near.x, near.y);
	arr[9].position = sf::Vector2f(Constants::window.x, near.y);
	arr[10].position = sf::Vector2f(Constants::window.x, far.y);
	arr[11].position = sf::Vector2f(far.x, far.y);

	for(int i = 0; i < 4;  i++) arr[i].color = offroadColor;
	for(int i = 4; i < 8;  i++) arr[i].color = roadColor;
	for(int i = 8; i < 12; i++) arr[i].color = offroadColor;
}
