#pragma once

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void render();
    void initBackground();

    sf::RenderWindow window;
    sf::Sprite m_backgroundSprite;
};


