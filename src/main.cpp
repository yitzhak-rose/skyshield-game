#include "App/Game.h"
#include <exception>
#include <iostream>

int main() try {
	Game game;
	game.run();

	return EXIT_SUCCESS;
}
catch (const std::exception& e) {
	std::cerr << "Fatal error: " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Fatal error: unknown exception" << std::endl;
	return EXIT_FAILURE;
}

