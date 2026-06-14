/*#pragma once

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
};*/


#pragma once

#include <SFML/Graphics.hpp>
#include "World.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void handleInput();
    void update(float dt);
    void render();
    void initBackground();

    sf::RenderWindow window;
    sf::Sprite m_backgroundSprite;
    World m_world;
    sf::Clock m_clock;
};