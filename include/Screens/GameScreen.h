#pragma once

#include "Screens/Screen.h"
#include "Gameplay/World.h"
#include "Gameplay/GameState.h"
#include "Gameplay/LevelConfig.h"
#include "Screens/Menubar.h"             
#include "Enums/LevelResult.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

class GameScreen : public Screen {
public:
    /// @brief Creates the gameplay screen and its initial start overlay.
    /// @param config Level configuration used to create the world.
    /// @param gameState Shared mutable game state.
    /// @param quitGame Callback used when a screen-owned overlay asks to quit.
    GameScreen(const LevelConfig& config, GameState& gameState, std::function<void()> quitGame);

    /// @brief Handles gameplay input when no overlay is active.
    void handleEvent(const sf::Event& event) override;

    /// @brief Updates the world, HUD, or active overlay.
    void update(float deltaTime) override;

    /// @brief Draws the world, HUD, and any active overlay.
    void draw(sf::RenderWindow& window) override;

    /// @brief Returns true when the level was won and Game should advance screens.
    bool isFinished() const override { return m_world->getResult() == LevelResult::Won; }

    /// @brief Returns the current world result.
    LevelResult getResult() const override { return m_world->getResult(); }

private:
    /// @brief Applies real-time movement input to the drone.
    void handleInput();

    /// @brief Shows the loss summary and restart/quit actions.
    void showGameOverOverlay();

    /// @brief Recreates the world for the current level.
    void restartLevel();

    LevelConfig m_config;

    GameState& m_gameState;

    std::function<void()> m_quitGame;

    /// @brief Owned through unique_ptr so restart can replace the whole world cleanly.
    std::unique_ptr<World> m_world;

    std::unique_ptr<MenuBar> m_menuBar;
};
