#pragma once

#include "Screens/Screen.h"
#include "Gameplay/World.h"
#include "Gameplay/LevelConfig.h"
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
