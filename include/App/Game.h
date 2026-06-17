#pragma once

#include <SFML/Graphics.hpp>
#include "Gameplay/GameState.h"
#include "Gameplay/World.h"

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
    GameState m_gameState;
    World m_world;
    sf::Clock m_clock;
};
