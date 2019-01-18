#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <string>

/**
 *	@brief Interface for loading resources
 */
class Resource
{
public:
	/**
	 *	@brief Method to load resource from file
	 *	@param str File path.
	 *	@return `true` if operation succeeded.
	 */
	virtual bool loadFromFile(const std::string & str) = 0;
};


/**
 *	@brief Resource class designed to load a `sf::Texture`.
 */
class MyTexture : public Resource, public sf::Texture
{
public:
	/**
	 *	@brief Method to load resource from file
	 *	@param str File path.
	 *	@return `true` if operation succeeded.
	 */
	virtual bool loadFromFile(const std::string & str)
	{
		return sf::Texture::loadFromFile(str);
	}
};

/**
 *	@brief Resource class designed to load a `sf::Font`.
 */
class MyFont : public Resource, public sf::Font
{
public:
	/**
	 *	@brief Method to load resource from file
	 *	@param str File path.
	 *	@return `true` if operation succeeded.
	 */
	virtual bool loadFromFile(const std::string & str)
	{
		return sf::Font::loadFromFile(str);
	}
};

/**
 *	@brief Resource class designed to load a `sf::SoundBuffer`.
 */
class MySoundBuffer : public Resource, public sf::SoundBuffer
{
public:
	/**
	 *	@brief Method to load resource from file
	 *	@param str File path.
	 *	@return `true` if operation succeeded.
	 */
	virtual bool loadFromFile(const std::string & str)
	{
		return sf::SoundBuffer::loadFromFile(str);
	}
};

#endif

