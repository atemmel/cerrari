#include <SFML/Graphics.hpp>

#include <cmath>
#include <iostream>
#include <vector>

const static sf::VideoMode defaultMode(1024, 768);

static bool fullscreen = 0;

sf::RectangleShape background(sf::Vector2f(1024.f, 768.f));

float fov = 100.f;
float roadHeight = 1000.f;
sf::Vector3f camera	= {0.f, 0.f, 1000.f};

namespace Curve
{
	float None = 0.f;
	float Weak = 5.f;
	float Medium = 10.f;
	float Sharp = 20.f;
};

struct Segment
{
	sf::Vector3f start, finish;
	float curve;
};

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

float easeIn(float a, float b, float scale)
{
	return a + (b - a) * (scale * scale);
}

float easeOut(float a, float b, float scale)
{
	const float diff = 1.f - scale;
	return a + (b - a) * (1.f - diff * diff);
}

float getDecimal(float a)
{
	int b = a;
	return a - b;
}

float normalize(float min, float max, float value)
{
	return (value - min) / (max - min);
}

sf::Vector3f worldToScreen(const sf::Vector3f & world)
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
	const float roadWidth = 2000.f;

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
	sf::View view(window.getView());
	sf::Texture playerTexture;
	sf::Sprite playerSprite;

	background.setFillColor(sf::Color(80, 0, 200)); //TODO: Remove constant
	window.setFramerateLimit(60u);
	playerTexture.loadFromFile("ferrari.png");
	playerSprite.setTexture(playerTexture);

	{
		auto size = background.getSize();
		view.setSize(size);
		view.setCenter(size / 2.f);
	}

	sf::VertexArray quad(sf::PrimitiveType::Quads, 4u);
	
	size_t n_segs = 500;
	float segmentLength = 1600.f;
	std::vector<Segment> segments(n_segs);

	for(size_t i = 0u; i < segments.size(); i++)
	{
		auto & seg = segments[i];
		seg.start.y = seg.finish.y = roadHeight;
		seg.start.z = i * -segmentLength+segmentLength;
		seg.finish.z = (i + 1) * -segmentLength+segmentLength;
		seg.curve = Curve::None;
	}

	for(size_t i = 30u; i < 50u; i++) 
	{
		float s = normalize(30u, 50u, i);
		segments[i].curve = easeIn(Curve::None, -Curve::Medium, s);
	}
	for(size_t i = 50u; i < 70u; i++) 
	{
		segments[i].curve = -Curve::Medium;
	}
	for(size_t i = 70u; i < 90u; i++) 
	{
		float s = normalize(70u, 90u, i);
		segments[i].curve = easeOut(-Curve::Medium, Curve::None, s);
	}

	float velocity = 0.f, maxVelocity = 200.f;
	float turnVelocity = 40.f;

	//sf::CircleShape dbg1(4.f);
	//auto dbg2 = dbg1;
	//dbg1.setFillColor(sf::Color::Green);
	//dbg2.setFillColor(sf::Color::Red);

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
		
		//---------

		window.setView(view);
		window.clear();
		window.draw(background);

		float base = camera.z / -segmentLength;
		float ddx = segments[base].curve;
		float dx = -getDecimal(base) * ddx;

		for(size_t i = base; i < segments.size() && i < base + 30; i++)
		{
			auto & seg = segments[i];

			//Behind us
			if(seg.finish.z > segmentLength) continue;

			auto screen1 = worldToScreen(seg.start);
			auto screen2 = worldToScreen(seg.finish);

			screen1.x += dx;
			screen2.x += dx + ddx;

			dx += ddx;
			ddx = seg.curve;

			//Too far ahead
			if(screen2.y > 768.f || screen2.z < 60.f) continue; //TODO: Remove constant

			//dbg1.setPosition(screen1.x, screen1.y);

			moveQuad(quad, {screen1.x, screen1.y}, screen1.z, {screen2.x, screen2.y}, screen2.z,
					i % 2 == 0 ? sf::Color(100, 100, 100) : sf::Color(120, 120, 120)); //TODO: Remove constant

			window.draw(quad);
			//window.draw(dbg1);
		}

		window.display();
	}
}
