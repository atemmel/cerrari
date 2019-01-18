#ifndef GAME_HPP
#define GAME_HPP

#include "road.hpp"
#include "player.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

/**
 *		@brief Class for managing most of the game content..
 */
class Game
{
public:
	/**
	 *		@brief Parametrized constructor.
	 *		@param window The window the game will operate on.
	 *		@param seed A seed for the game to generate a road from.
	 *		@param length Track length
	 */
	Game(sf::RenderWindow & window, Road::Seed seed, unsigned length);

	/**
	 *		@brief Reads inputs from the associated `sf::RenderWindow.`
	 *		@note Should be called regularly to keep the program responsive.
	 */
	void readInputs();

	/**
	 *		@brief Updates the state of the game.
	 */
	void update();

	/**
	 *		@brief Renders the game unto the associated `sf::RenderWindow.`
	 */
	void render();
private:

	/**
	 *		@brief Translates logical coordinates to renderable coordinates.
	 *		@param world Logical coordinate to translate
	 *		@param width Width of the segment associated with the coordinate
	 *		@return A centered and translated coordinate.
	 *		@note The `z` field in the return value contains the width of the 
	 *		translated `sf::Vector3f`
	 */
	sf::Vector3f worldToScreen(const sf::Vector3f & world, float width);

	sf::RenderWindow & m_window;
	sf::Vector3f m_camera	= {0.f, 0.f, 1000.f};
	sf::Texture m_playerTexture, m_bgTexture;
	sf::Sprite m_background;
	sf::VertexArray m_quad;
	sf::SoundBuffer m_brrrBuffer;
	sf::Sound m_brrr;
	sf::Music m_africa;
	sf::Font m_font;
	sf::Text m_text;
	sf::Clock m_clock;

	Road m_road;
	Road::Segments m_segments;
	Player m_player;
	bool m_finished = 0;

	constexpr static float fov = 100.f;
};

#endif
