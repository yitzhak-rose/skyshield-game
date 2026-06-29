#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "App/State/GameState.h"
#include "Shop/ShopData.h"
#include "Screens/Core/Screen.h"

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
        Game,
        Shop
    };

    void initBackground();

    /// @brief Loads saved persistent state before the first screen is shown.
    void loadSavedData();

    /// @brief Replaces the current screen with the start screen and marks it active.
    void showStartScreen();

    /// @brief Replaces the current screen with the gameplay screen and marks it active.
    void showGameScreen();

    /// @brief Replaces the current screen with the shop screen and marks it active.
    void showShopScreen();

    /// @brief Writes the current user data to the save file.
    void saveUserData();

    /// @brief Applies selected shop assets to the runtime resource maps.
    void syncShopSelections();

    /// @brief Polls SFML events, handles window-owned events, and dispatches safe context to screens.
    void handleEvents();

    /// @brief Builds event data that screens need without exposing the mutable window.
    ScreenEventContext makeEventContext(const sf::Event& event) const;

    /// @brief Executes a pending screen command only if the active screen is allowed to request it.
    void handleScreenCommand();

    /// @brief Returns true when a requested command is valid for the current active screen.
    bool isCommandAllowed(ScreenCommand command) const;

    /// @brief Applies a command after it passes the active-screen policy check.
    void applyScreenCommand(ScreenCommand command);

    void update(float dt);

    /// @brief Saves user data when coins or shop selections have changed since the last write.
    void saveUserDataIfChanged();

    /// @brief Reloads JSON-backed data if its files changed on disk.
    void refreshDataIfChanged();

    void render();

    sf::RenderWindow window;
    sf::Clock m_clock;
    sf::Sprite m_backgroundSprite;

    GameState m_gameState;
    UserData m_lastSavedUserData;
    ActiveScreen m_activeScreen = ActiveScreen::Start;

    std::unique_ptr<Screen> m_screen;
};
