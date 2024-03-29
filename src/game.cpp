#include "game.hpp"

Game::Game(sf::RenderWindow & window, Road::Seed seed, unsigned length)
		: m_window(window), m_road(20, 40, seed), m_quad(sf::PrimitiveType::Quads, 12u)
{
	m_resources.loadFromFile<MyFont>("resources/font.ttf");
	m_resources.loadFromFile<MyTexture>("resources/bg.png");
	m_resources.loadFromFile<MyTexture>("resources/ferrari.png");
	m_resources.loadFromFile<MySoundBuffer>("resources/brrr.ogg");

	m_africa.openFromFile("resources/music/africa.ogg");
	m_resources.access<MyTexture>("resources/bg.png")->setRepeated(true);

	m_text.setFont(*m_resources.access<MyFont>("resources/font.ttf") );
	m_background.setTexture(*m_resources.access<MyTexture>("resources/bg.png") );
	m_player.sprite.setTexture(*m_resources.access<MyTexture>("resources/ferrari.png") );
	m_carSound.setBuffer(*m_resources.access<MySoundBuffer>("resources/brrr.ogg") );

	m_background.setScale(1.2f, 1.2f);
	m_text.setCharacterSize(80);
	m_text.setOutlineColor(sf::Color::Black);
	m_text.setOutlineThickness(5u);
	m_speedometer = m_text;
	m_speedometer.setCharacterSize(40u);
	m_speedometer.setOutlineThickness(2u);

	m_text.setPosition(100.f, 100.f);
	m_speedometer.setPosition(10.f, 10.f);
		
	m_player.sprite.setTextureRect(sf::IntRect(m_player.spriteNormalPos, m_player.spriteDim) );
	m_player.sprite.setScale(4.f, 4.f);
	m_player.position = {0.f, 0.f, -(Constants::Road::SegmentLength * 3.2f)};

	sf::View view(m_window.getView());

	{
		auto size = Constants::window;
		view.setSize(size);
		view.setCenter(size / 2.f);
		m_window.setView(view);

		auto texRect = m_background.getTextureRect();
		texRect.top = 80.f;
		m_background.setTextureRect(texRect);
	}

	m_carSound.setLoop(true);
	m_carSound.setVolume(10.f);

	m_africa.setVolume(30.f);
	m_africa.setLoop(true);
	m_africa.play();
	
	m_segments = m_road.generate(length);
}

void Game::readInputs()
{
	sf::Event event;

	while(m_window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::Closed:
				m_window.close();
				break;

			case sf::Event::KeyPressed:
				switch(event.key.code)
				{
					case sf::Keyboard::Key::Escape:
					case sf::Keyboard::Key::F8:
						m_window.close();
						break;
				}
				break;
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_player.accelerate();
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_player.brake();
	}
	else m_player.deaccelerate();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_player.turn(Player::Direction::Left);
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_player.turn(Player::Direction::Right);
	}

}

void Game::update()
{
	if(m_finished) return;

	float base = m_camera.z / -Constants::Road::SegmentLength;
	if(m_segments.size() <= base + 30)
	{
		m_player.stop();

		sf::Time time = m_clock.restart();

		int minutes = time.asSeconds() / 60.f;
		int seconds = time.asSeconds() - 60.f * minutes;

		std::string strMinutes = std::to_string(minutes);
		std::string strSeconds = std::to_string(seconds);

		if(strSeconds.size() == 1) strSeconds = '0' + strSeconds;

		m_text.setString(
				"SESSION COMPLETED!\n\n"
				"WELL DONE!\n\n"
				"TOTAL TIME:\t" 
				 + strMinutes + ':'
				 + strSeconds + "\n\n\n\n"
				"PRESS ESC TO EXIT"
				);

		m_speedometer.setString("");
		m_finished = true;
		return;
	}

	m_player.update();

	if(m_player.acceleration > 0.f && m_carSound.getStatus() != sf::Sound::Status::Playing) m_carSound.play();
	else if(m_player.acceleration <= 0.f) m_carSound.stop();

	m_camera.z = m_player.position.z + (Constants::Road::SegmentLength * 3.2f) + 1000.f;
	m_camera.x = m_player.position.x;

	m_speedometer.setString(std::to_string(int(-m_player.velocity.z / 5.f) ) + "km/h");
}

void Game::render()
{
	m_window.clear();

	float base = m_camera.z / -Constants::Road::SegmentLength;
	float playerDepth = m_player.position.z / -Constants::Road::SegmentLength;
	float prevHeight = m_segments[base].y;
	float ddx = m_segments[base].x;
	float dx = Math::getDecimal(base) * -ddx;
	float minY = m_segments[base].y;

	m_window.draw(m_background);

	auto rect = m_background.getTextureRect();
	rect.left = m_camera.x * 0.1f;
	rect.top = minY * 0.02f + 40.f;
	m_background.setTextureRect(rect);


	m_camera.x = m_camera.x - (1.5f * m_segments[base].x) * m_player.velocity.z/-m_player.maxVelocity;
	m_camera.y = Math::interpolate(m_segments[playerDepth].y, m_segments[playerDepth + 1].y, Math::getDecimal(playerDepth) ) 
		- Constants::Road::MaxHeight;


	for(size_t i = base; i < m_segments.size() && i < base + 30; i++)
	{
		auto & seg = m_segments[i];
		auto & seg2 = m_segments[i + 1];

		auto screen1 = worldToScreen(seg, Constants::Road::Width);
		auto screen2 = worldToScreen(seg2, Constants::Road::Width);

		screen1.x += dx;
		screen2.x += dx + ddx;

		dx += ddx;
		ddx = seg.x;

		//Too far ahead
		if(	screen2.y > Constants::window.y 		||	//Utanför skärmens gränser
			minY < screen2.y  						||	//Bakom en kulle
			screen2.z < Constants::window.x / 60.f		//För smal för att renderas	
		) continue;

		if(minY > screen2.y) minY = screen2.y;

		Utils::moveQuad(m_quad, {screen1.x, screen1.y}, screen1.z, {screen2.x, screen2.y}, screen2.z,
				i % 2 == 0 ? sf::Color(100, 100, 100) : sf::Color(120, 120, 120), 
				i % 2 == 0 ? sf::Color(87 + 40, 66 + 40, 25) : sf::Color(63, 49, 15)
		); 

		m_window.draw(m_quad);
	}

	m_player.sprite.setPosition( (Constants::window.x - m_player.sprite.getGlobalBounds().width) * 0.5f, 
			Constants::window.y - m_player.sprite.getGlobalBounds().height + sinf(m_player.position.z * 0.5f) );

	m_window.draw(m_player.sprite);
	m_window.draw(m_text);
	m_window.draw(m_speedometer);

	m_window.display();
}

sf::Vector3f Game::worldToScreen(const sf::Vector3f & world, float width)
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
	const sf::Vector3f outCamera = world - m_camera;
	const float scale = d / outCamera.z;

	const sf::Vector2f proj(outCamera.x * scale, outCamera.y * scale);
	const sf::Vector2f halfSize = Constants::window * 0.5f;
	sf::Vector3f screen = {halfSize.x, halfSize.y, 0.f};
	screen.x += halfSize.x * proj.x;
	screen.y += halfSize.y * proj.y;
	screen.z  = scale * width * halfSize.x;

	return screen;
}
