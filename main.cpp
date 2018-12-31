#include <SFML/Graphics.hpp>
#include "road.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

const static sf::VideoMode defaultMode(1024, 768);

sf::RectangleShape background(sf::Vector2f(1024.f, 768.f));

static bool fullscreen = 0;

float fov = 100.f;
sf::Vector3f camera	= {0.f, 0.f, 1000.f};
sf::Vector3f player = {0.f, 0.f, -(Constants::Road::SegmentLength * 3.2f)};


void moveQuad(sf::VertexArray & arr, sf::Vector2f near, float nearWidth, 
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
	sf::Sprite playerSprite;

	background.setFillColor(sf::Color(0, 0, 0)); //TODO: Remove constant
	window.setFramerateLimit(60u);
	playerTexture.loadFromFile("ferrari.png");
	playerSprite.setTexture(playerTexture);

	{
		auto size = background.getSize();
		view.setSize(size);
		view.setCenter(size / 2.f);
	}

	sf::VertexArray quad(sf::PrimitiveType::Quads, 4u);
	sf::VertexArray grid(sf::PrimitiveType::Lines, 30u);
	
	Road bob(20, 40, 3933873847);
	//Road bob(20, 40, std::random_device()());
	auto segments = bob.generate(320);
	
	float velocity = 0.f, maxVelocity = 200.f;
	float turnVelocity = 40.f;

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
			velocity = maxVelocity;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			velocity = -maxVelocity;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			velocity = 500.f;
		}
		else velocity = 0.f;

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			camera.x -= turnVelocity;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			camera.x += turnVelocity;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
		{
			fov -= 0.01;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			fov += 0.01;
		}

		//---------

		camera.z -= velocity;
		player.z -= velocity;
		
		//---------

		window.setView(view);
		window.clear();
		window.draw(background);

		float base = camera.z / -Constants::Road::SegmentLength;
		float playerDepth = player.z / -Constants::Road::SegmentLength;
		float prevHeight = segments[base].y;
		float ddx = segments[base].x;
		float dx = Math::getDecimal(base) * -ddx;
		float minY = segments[base].y;

		camera.y = -Constants::Road::MaxHeight + Math::interpolate(segments[playerDepth].y, 
				segments[playerDepth + 1].y, Math::getDecimal(playerDepth) );

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
			if(	screen2.y > 768.f ||	//Utanför skärmens gränser
				minY < screen2.y  ||	//Bakom en kulle
				screen2.z < 60.f		//För smal för att renderas	
			) continue; //TODO: Remove constant

			moveQuad(quad, {screen1.x, screen1.y}, screen1.z, {screen2.x, screen2.y}, screen2.z,
					i % 2 == 0 ? sf::Color(100, 100, 100) : sf::Color(120, 120, 120)); //TODO: Remove constant

			if(minY > screen2.y) minY = screen2.y;

			window.draw(quad);
		}

		window.display();
	}
}
