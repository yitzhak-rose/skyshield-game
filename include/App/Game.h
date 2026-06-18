#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Gameplay/GameState.h"
#include "Gameplay/LevelConfig.h"
#include "Screens/Screen.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void initBackground();
    void startStage(int index);
    void advanceStage();

    sf::RenderWindow window;
    sf::Clock m_clock;
    sf::Sprite m_backgroundSprite;

    GameState m_gameState;
    std::vector<LevelConfig> m_stages;
    int m_currentStage = 0;

    std::unique_ptr<Screen> m_screen;
};