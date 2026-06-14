#pragma once

#include "Screen.h"
#include "World.h"
#include "LevelConfig.h"
#include <SFML/Graphics.hpp>

class GameScreen: public Screen {
public:
    GameScreen();

    void startGame();
    void draw(sf::RenderWindow& window) override;
    void update(float deltaTime) override;


private:
    World m_world;

};
