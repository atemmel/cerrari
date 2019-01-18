#include "game.hpp"

#include <getopt.h>

static void die(const std::string & str);

static unsigned readInt(const std::string & str);

static void help();

int main(int argc, char** argv)
{
	Road::Seed seed = -1;
	unsigned length = 4000u;

	const char* short_opts = "hs:l:";
	const struct option long_opts[] =
	{
		{"help", 	0, NULL, 'h'},
		{"seed", 	1, NULL, 's'},
		{"length",	1, NULL, 'l'}
	};

	char arg = 0;

	while( (arg = getopt_long(argc, argv, short_opts, long_opts, nullptr) ) != -1)
	{
		switch(arg)
		{
			case 's':
				seed = readInt(optarg);
				break;
			case 'l':
				length = readInt(optarg);
				break;
			case 'h':
				help();
				break;
			case '?':
			default:
				die("Error: Unrecognized argument");
		}
	}

	if(seed == -1) seed = std::random_device()();
	if(length == 0) die("Error: Invalid length");

	sf::RenderWindow window(sf::VideoMode(Constants::window.x, Constants::window.y),  "");
	window.setFramerateLimit(60u);

	Game game(window, seed, length);
	
	while(window.isOpen() )
	{
		game.readInputs();

		game.update();

		game.render();
	}
}

static void die(const std::string & str)
{
	std::cerr << str << '\n';
	exit(EXIT_FAILURE);
}

static unsigned readInt(const std::string & str)
{
	try
	{
		return stoi(str);
	}
	catch(std::invalid_argument e)
	{
		die("Error: \"" + str + "\" could not be converted to an integer");
	}

	return 0;
}

static void help()
{
	std::cout <<
		"cerrari is a causal game in which you pilot a ferrari\n"
		"through a procedurally generated course\n\n"
		"Options:\n"
		"-h, --help\t: Display this text\n"
		"-l, --length\t: Set length of track\n"
		"-s, --seed\t: Set track seed\n"
		"\nIf either the seed or the length is not given, the\n"
		"program will provide a default length as well as \n"
		"generate a random seed.\n";

	exit(EXIT_SUCCESS);
}
