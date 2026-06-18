#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Gameplay/GameState.h"
#include "Gameplay/World.h"
#include "Screens/Menubar.h"

class Game {
public:
    Game();
    ~Game();
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
    std::unique_ptr<MenuBar> m_menuBar;
    sf::Clock m_clock;
};
