#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "game.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
	sf::RenderWindow window(sf::VideoMode(Constants::window.x, Constants::window.y),  "");
	window.setFramerateLimit(60u);

	Game game(window, std::random_device()());
	
	while(window.isOpen())
	{

		game.readInputs();

		//---------//

		game.update();

		//---------//
		
		game.render();

	}
}
