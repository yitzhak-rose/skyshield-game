#include "Game.h"

Game::Game()
    : window(sf::VideoMode({800u, 600u}), "SFML Window") {
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.display();
}

