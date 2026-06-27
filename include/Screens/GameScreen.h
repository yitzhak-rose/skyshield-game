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
    explicit GameScreen(GameState& gameState, const std::vector<LevelConfig>& stages);

    /// @brief Handles gameplay input when no overlay is active.
    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;

    /// @brief Updates the world, HUD, or active overlay.
    void update(float deltaTime) override;

    /// @brief Draws the world, HUD, and any active overlay.
    void draw(sf::RenderWindow& target) override;

private:
    /// @brief Shows the loss summary and restart/quit actions.
    void showGameOverOverlay();

    /// @brief Returns true when the current stage is the last configured stage.
    bool isFinalStage() const;

    /// @brief Shows the final completion summary and quit action.
    void showGameCompletedOverlay();

    /// @brief Shows the level-completed summary and continue action.
    void showLevelCompletedOverlay();

    /// @brief Returns summary lines reused by completion overlays.
    std::vector<std::string> makeLevelSummaryLines() const;

    /// @brief Advances to the next stage after the level-completed overlay.
    void advanceStage();

    /// @brief Loads the current stage into a fresh world.
    void startCurrentStage();

    /// @brief Recreates the world for the current level.
    void restartLevel();

    /// @brief Drives the post-hit screen shake (drops to 0 when idle).
    void updateShake(float dt);

    /// @brief How much time is left in the current screen shake.
    float m_shakeTimer = 0.f;

    /// @brief The damage value seen last frame — a rise means a fresh hit.
    float m_lastDamageTaken = 0.f;

    GameState& m_gameState;
    bool m_completionOverlayShown = false;
    std::vector<LevelConfig> m_stages;
    int m_currentStage = 0;

    /// @brief Owned through unique_ptr so restart can replace the whole world cleanly.
    std::unique_ptr<World> m_world;
    std::unique_ptr<MenuBar> m_menuBar;
};