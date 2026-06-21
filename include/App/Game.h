#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Gameplay/GameState.h"
#include "Screens/Screen.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void update(float dt);
    void render();
    void initBackground();
    void showStartScreen();
    void showGameScreen();
    void requestStartScreen();

    sf::RenderWindow window;
    sf::Clock m_clock;
    sf::Sprite m_backgroundSprite;

    GameState m_gameState;
    bool m_startScreenRequested = false;

    std::unique_ptr<Screen> m_screen;
};
