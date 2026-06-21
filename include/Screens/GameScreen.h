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
#include <string>
#include <vector>

class GameScreen : public Screen {
public:
    /// @brief Creates the gameplay screen for a full run through the configured stages.
    /// @param gameState Shared mutable game state.
    /// @param returnToStart Callback used after the full game is completed.
    GameScreen(GameState& gameState,
               std::function<void()> returnToStart);

    /// @brief Handles gameplay input when no overlay is active.
    void handleEvent(const sf::Event& event) override;

    /// @brief Updates the world, HUD, or active overlay.
    void update(float deltaTime) override;

    /// @brief Draws the world, HUD, and any active overlay.
    void draw(sf::RenderWindow& window) override;

private:
    /// @brief Applies real-time movement input to the drone.
    void handleInput();

    /// @brief Shows the loss summary and restart/quit actions.
    void showGameOverOverlay();

    /// @brief Shows the level-completed summary and continue action.
    void showLevelCompletedOverlay();

    /// @brief Shows the final completion summary and quit action.
    void showGameCompletedOverlay();

    /// @brief Loads the current stage into a fresh world.
    void startCurrentStage();

    /// @brief Advances to the next stage after the level-completed overlay.
    void advanceStage();

    /// @brief Returns true when the current stage is the last configured stage.
    bool isFinalStage() const;

    /// @brief Returns summary lines reused by completion overlays.
    std::vector<std::string> makeLevelSummaryLines() const;

    /// @brief Recreates the world for the current level.
    void restartLevel();

    GameState& m_gameState;

    std::function<void()> m_returnToStart;

    bool m_completionOverlayShown = false;

    std::vector<LevelConfig> m_stages;

    int m_currentStage = 0;

    /// @brief Owned through unique_ptr so restart can replace the whole world cleanly.
    std::unique_ptr<World> m_world;

    std::unique_ptr<MenuBar> m_menuBar;
};
