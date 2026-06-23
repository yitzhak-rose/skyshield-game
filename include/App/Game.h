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
    /// @brief Identifies which top-level screen currently owns user interaction.
    ///
    /// Game uses this as a command policy boundary. Screens may request commands,
    /// but Game only executes commands that are valid for the current screen.
    enum class ActiveScreen {
        Start,
        Game
    };

    void update(float dt);
    void render();
    void initBackground();

    /// @brief Replaces the current screen with the start screen and marks it active.
    void showStartScreen();

    /// @brief Replaces the current screen with the gameplay screen and marks it active.
    void showGameScreen();

    /// @brief Loads saved persistent state before the first screen is shown.
    void loadSavedData();

    /// @brief Saves coins when the total has changed since the last write.
    void saveCoinsIfChanged();

    /// @brief Writes the current coin total to the save file.
    void saveCoins();

    /// @brief Polls SFML events, handles window-owned events, and dispatches safe context to screens.
    void handleEvents();

    /// @brief Executes a pending screen command only if the active screen is allowed to request it.
    void handleScreenCommand();

    /// @brief Returns true when a requested command is valid for the current active screen.
    bool isCommandAllowed(ScreenCommand command) const;

    /// @brief Applies a command after it passes the active-screen policy check.
    void applyScreenCommand(ScreenCommand command);

    /// @brief Builds event data that screens need without exposing the mutable window.
    ScreenEventContext makeEventContext(const sf::Event& event) const;

    sf::RenderWindow window;
    sf::Clock m_clock;
    sf::Sprite m_backgroundSprite;

    GameState m_gameState;
    int m_lastSavedCoins = 0;
    ActiveScreen m_activeScreen = ActiveScreen::Start;

    std::unique_ptr<Screen> m_screen;
};
