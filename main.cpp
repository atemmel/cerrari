#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <vector>

const static sf::VideoMode defaultMode(1024, 768);

static bool fullscreen = 0;

sf::RectangleShape background(sf::Vector2f(1024.f, 768.f));

float fov = 100.f;
float roadHeight = 220.f;
sf::Vector3f camera	= {0.f, 0.f, 1000.f};

void moveQuad(sf::VertexArray & arr, sf::Vector2f near, float nearWidth, sf::Vector2f far, float farWidth, sf::Color color)
{
	const sf::Vector2f halfNearWidth	= {nearWidth	* 0.5f, 0.f};
	const sf::Vector2f halfFarWidth		= {farWidth		* 0.5f, 0.f};

	arr[0].position = sf::Vector2f(near - halfNearWidth);
	arr[1].position = sf::Vector2f(near + halfNearWidth);
	arr[2].position = sf::Vector2f(far + halfFarWidth);
	arr[3].position = sf::Vector2f(far - halfFarWidth);

	for(int i = 0; i < 4; i++) arr[i].color = color;
}

sf::Vector3f worldToScreen(sf::Vector3f world)
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

	float d = 1.f/tanf(fov * 0.5f);
	sf::Vector3f outCamera = world - camera;

	float scale = d / outCamera.z;
	float roadWidth = 600.f;

	sf::Vector2f proj(outCamera.x * scale, outCamera.y * scale);
	sf::Vector2f halfSize = background.getSize() * 0.5f;
	sf::Vector3f screen = {halfSize.x, halfSize.y, 0.f};
	screen.x += halfSize.x * proj.x;
	screen.y += halfSize.y * proj.y;
	screen.z  = scale * roadWidth * halfSize.x;

	return screen;
}

int main()
{
	sf::RenderWindow window(defaultMode, "");

	background.setFillColor(sf::Color(80, 0, 200));
	window.setFramerateLimit(60u);

	sf::View view = window.getView();
	{
		auto size = background.getSize();
		view.setSize(size);
		view.setCenter(size / 2.f);
	}

	sf::VertexArray quad(sf::PrimitiveType::Quads, 4u);
	
	size_t n_segs = 128u;
	std::vector<sf::Vector3f> segments(n_segs);

	for(size_t i = 0u; i < segments.size(); i++)
	{
		sf::Vector3f & seg = segments[i];
		seg.y = roadHeight;
		seg.z = (i / n_segs * 200.f);
	}

	sf::Vector3f start = {0.f, roadHeight, 0.f}, 
		end = {0.f, roadHeight, 200.f};

	float velocity = 0.f, maxVelocity = 20.f;

	sf::CircleShape dbg1(4.f);
	auto dbg2 = dbg1;
	dbg1.setFillColor(sf::Color::Green);
	dbg2.setFillColor(sf::Color::Red);

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
		else velocity = 0.f;

		//---------

		start.z += velocity;
		end.z += velocity;
		
		auto screen1 = worldToScreen(start);
		auto screen2 = worldToScreen(end);
		dbg1.setPosition(screen1.x, screen1.y);
		dbg2.setPosition(screen2.x, screen2.y);

		moveQuad(quad,
				{screen1.x, screen1.y}, screen1.z,
				{screen2.x, screen2.y}, screen2.z,
				sf::Color(100, 100, 100));

		//---------

		window.setView(view);
		window.clear();
		window.draw(background);

		window.draw(quad);
		window.draw(dbg1);
		window.draw(dbg2);
		window.display();
	}
}
