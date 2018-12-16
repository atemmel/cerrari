#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>

const static sf::VideoMode defaultMode(1024, 768);

static bool fullscreen = 0;

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

int main()
{
	sf::RectangleShape background(sf::Vector2f(1024.f, 768.f));
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
	
	float fov = 100.f;
	float d = 1.f/tanf(fov * 0.5f);
	sf::Vector3f camera	= {0.f, 0.f, 1000.f};
	sf::Vector3f world	= {0.f, 200.f, 0.f};

	sf::Vector3f outCamera = world - camera;

	float scale = d / outCamera.z;
	float roadWidth = 800.f;

	sf::Vector2f proj(outCamera.x * scale, outCamera.y * scale);
	sf::Vector2f halfSize = background.getSize() * 0.5f;
	sf::Vector3f screen = {halfSize.x, halfSize.y, 0.f};
	screen.x += halfSize.x * proj.x;
	screen.y += halfSize.y * proj.y;
	screen.z  = scale * roadWidth * halfSize.x;

	std::cout << screen.x << ' ' << screen.y << '\n';

	sf::CircleShape dbg(4.f);
	dbg.setFillColor(sf::Color::Green);
	dbg.setPosition(screen.x, screen.y);

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

		//---------
		

		//---------

		window.setView(view);
		window.clear();
		window.draw(background);



		moveQuad(quad, 
			{512.f, 500.f}, 300.f, 
			{512.f, 400.f}, 200.f,
			sf::Color::White);

		window.draw(quad);
		window.draw(dbg);
		window.display();
	}
}
