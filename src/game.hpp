#ifndef GAME_HPP
#define GAME_HPP

#include "road.hpp"
#include "player.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

class Game
{
public:
	Game(sf::RenderWindow & window, Road::Seed seed);
	void readInputs();

	void update();

	void render();
private:

	sf::Vector3f worldToScreen(const sf::Vector3f & world, float width);

	Road m_road;
	sf::RenderWindow & m_window;
	sf::Vector3f m_camera	= {0.f, 0.f, 1000.f};
	sf::Texture m_playerTexture, m_bgTexture;
	sf::Sprite m_background;
	sf::VertexArray m_quad;
	sf::SoundBuffer m_brrrBuffer;
	sf::Sound m_brrr;
	sf::Music m_africa;
	Road::Segments m_segments;
	Player m_player;
	constexpr static float fov = 100.f;
};

#endif
