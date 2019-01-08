#include <SFML/Graphics.hpp>

#include "road.hpp"
#include "player.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

static constexpr int WindowX = 1280;
static constexpr int WindowY = 720;

const static sf::VideoMode defaultMode(WindowX, WindowY);

sf::RectangleShape background(sf::Vector2f(WindowX, WindowY) );

static bool fullscreen = 0;

float fov = 100.f;
sf::Vector3f camera	= {0.f, 0.f, 1000.f};

sf::Vector3f worldToScreen(const sf::Vector3f & world, float width)
{
	// h = camera height
	// w = camera to road distance
	// d = camera to screen distance (think z)
	//
	// z = distance from car to camera
	// y = screen coordinate
	// x = screen coordinate
	//
	// ---- Formula ----
	//
	// x = w * d/z
	// y = h * d/z
	// translate -> project -> scale
	// world -> camera -> screen
	//
	// carg, cres = camera pos arg/res
	// w = world pos
	// p = projection pos
	// s = screen pos
	// S = window size
	//
	// cres = carg - w;
	// p = cres * d / cres.z
	// s.x = (S.x/2) + (S.x/2 * p.x)
	// s.y = (S.y/2) - (S.y/2 * p.y)

	const float d = 1.f/tanf(fov * 0.5f);
	const sf::Vector3f outCamera = world - camera;
	const float scale = d / outCamera.z;

	const sf::Vector2f proj(outCamera.x * scale, outCamera.y * scale);
	const sf::Vector2f halfSize = background.getSize() * 0.5f;
	sf::Vector3f screen = {halfSize.x, halfSize.y, 0.f};
	screen.x += halfSize.x * proj.x;
	screen.y += halfSize.y * proj.y;
	screen.z  = scale * width * halfSize.x;

	return screen;
}

int main()
{
	sf::RenderWindow window(defaultMode, "");
	sf::View view(window.getView());
	sf::Texture playerTexture;
	Player player;

	background.setFillColor(sf::Color(0, 0, 0)); //TODO: Remove constant
	window.setFramerateLimit(60u);
	playerTexture.loadFromFile("ferrari.png");
	player.sprite.setTexture(playerTexture);
	player.sprite.setTextureRect(sf::IntRect(player.spriteNormalPos, player.spriteDim) );
	player.sprite.setScale(4.f, 4.f);
	player.position = {0.f, 0.f, -(Constants::Road::SegmentLength * 3.2f)};

	{
		auto size = background.getSize();
		view.setSize(size);
		view.setCenter(size / 2.f);
	}

	sf::VertexArray quad(sf::PrimitiveType::Quads, 4u);
	sf::VertexArray grid(sf::PrimitiveType::Lines, 30u);
	sf::VertexArray line(sf::PrimitiveType::Lines, 2u);
	line[0].color = line[1].color = sf::Color::Magenta;
	line[1].position.x = 1024;
	
	//Road bob(20, 40, 3933873847);
	Road bob(20, 40, std::random_device()());
	auto segments = bob.generate(1000);
	
	float velocity = 0.f, maxVelocity = 500.f;
	float acceleration = 0.f, accPerTick = 20.f;
	float turnVelocity = 16.f;
	//float velX = 0.f;
	float maxX = 2.f;

	while(window.isOpen())
	{
		sf::Event event;

		while(window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyPressed:
					switch(event.key.code)
					{
						case sf::Keyboard::Key::F8:
							window.close();
							break;
					}
					break;
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			acceleration = accPerTick;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			acceleration = -accPerTick * 0.5f;
		}
		else acceleration = 12 * -velocity / maxVelocity;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			player.velocity.x = -turnVelocity;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			player.velocity.x = turnVelocity;
		}
		else 
		{
			player.velocity.x = 0.f;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		{
			fov -= 0.01;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			fov += 0.01;
		}

		//---------//

		velocity += acceleration;
		velocity = std::clamp(velocity, 0.f, maxVelocity);
		player.velocity.x *= Math::normalize(0.f, maxVelocity, velocity);

		camera.x += player.velocity.x;
		camera.x = std::clamp(camera.x, -Constants::Road::Width, Constants::Road::Width);
		if(fabs(camera.x) > (Constants::Road::Width - player.sprite.getGlobalBounds().width) * 0.5f) velocity *= 0.92f;
		camera.z -= velocity;
		player.position.z -= velocity;
		player.update();

		
		//---------//

		window.setView(view);
		window.clear();
		window.draw(background);

		float base = camera.z / -Constants::Road::SegmentLength;
		float playerDepth = player.position.z / -Constants::Road::SegmentLength;
		float prevHeight = segments[base].y;
		float ddx = segments[base].x;
		float dx = Math::getDecimal(base) * -ddx;
		float minY = segments[base].y;
		camera.x = camera.x - (1.5f * segments[base].x);

		camera.y =  Math::interpolate(segments[playerDepth].y, segments[playerDepth + 1].y, Math::getDecimal(playerDepth) ) 
			- Constants::Road::MaxHeight;

		for(size_t i = base; i < segments.size() && i < base + 30; i++) //TODO: Remove constant
		{
			auto & seg = segments[i];
			auto & seg2 = segments[i + 1];

			auto screen1 = worldToScreen(seg, Constants::Road::Width);
			auto screen2 = worldToScreen(seg2, Constants::Road::Width);

			screen1.x += dx;
			screen2.x += dx + ddx;

			dx += ddx;
			ddx = seg.x;

			//Too far ahead
			if(	screen2.y > WindowY ||	//Utanför skärmens gränser
				minY < screen2.y  ||	//Bakom en kulle
				screen2.z < WindowX / 60.f		//För smal för att renderas	
			) continue; //TODO: Remove constant

			Utils::moveQuad(quad, {screen1.x, screen1.y}, screen1.z, {screen2.x, screen2.y}, screen2.z,
					i % 2 == 0 ? sf::Color(100, 100, 100) : sf::Color(120, 120, 120)); //TODO: Remove constant

			if(minY > screen2.y) minY = screen2.y;

			//if(i % 2 == 0)
			//{
				//line[0].position.y = line[1].position.y = screen1.y;
				//window.draw(line);
			//}

			window.draw(quad);
		}

		//auto v = worldToScreen(player, 0.f);
		player.sprite.setPosition( (WindowX - player.sprite.getGlobalBounds().width) * 0.5f, WindowY - player.sprite.getGlobalBounds().height);

		window.draw(player.sprite);

		window.display();
	}
}
