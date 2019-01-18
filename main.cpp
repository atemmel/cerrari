#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//#include "road.hpp"
//#include "player.hpp"
#include "game.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

//const static sf::VideoMode defaultMode(Constants::window.x, Constants::window.y);

//sf::Vector3f camera	= {0.f, 0.f, 1000.f};
//constexpr static float fov = 100.f;

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
