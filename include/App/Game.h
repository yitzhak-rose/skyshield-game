#pragma once

#include "App/State/GameState.h"
#include "Enums/ActiveScreen.h"
#include "Shop/ShopData.h"
#include "Screens/Core/Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>

class Game {
public:
#pragma region Public
    Game();
    ~Game();
    void run();
#pragma endregion

private:
#pragma region Private
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

    /// @brief Switches the loop into fatal overlay mode after a runtime exception.
    void enterFatalMode(const std::string& message);

    /// @brief Routes input only to the fatal overlay while fatal mode is active.
    void handleFatalEvents();

    void update(float dt);

    /// @brief Saves user data when coins or shop selections have changed since the last write.
    void saveUserDataIfChanged();

    void render();

    /// @brief Draws the last known screen state and fatal overlay.
    void renderFatalOverlay();

    sf::RenderWindow window;
    sf::Clock m_clock;
    sf::Sprite m_backgroundSprite;

    GameState m_gameState;
    UserData m_lastSavedUserData;
    ActiveScreen m_activeScreen = ActiveScreen::Start;
    bool m_fatalMode = false;

    std::unique_ptr<Screen> m_screen;
    std::unique_ptr<OverlayView> m_fatalOverlay;
#pragma endregion
};
