#pragma once

#include "Screens/Screen.h"
#include "Gameplay/World.h"
#include "Gameplay/GameState.h"
#include "Gameplay/LevelConfig.h"
#include "Screens/Menubar.h"
#include "Enums/LevelResult.h"
#include <SFML/Graphics.hpp>
#include <memory>

class GameScreen : public Screen {
public:
    GameScreen(const LevelConfig& config, GameState& gameState);

    void handleEvent(const sf::Event& event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    bool isFinished() const override { return m_world.getResult() != LevelResult::Running; }
    LevelResult getResult() const override { return m_world.getResult(); }

private:
    void handleInput();

    World m_world;
    std::unique_ptr<MenuBar> m_menuBar;
};