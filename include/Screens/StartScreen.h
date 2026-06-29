#pragma once

#include "Enums/StartScreenButton.h"
#include "Gameplay/Coin.h"
#include "Screens/Core/Screen.h"

#include <SFML/Graphics.hpp>
#include <functional>

class GameState;

/// @brief Opening screen with start, shop, and settings actions.
class StartScreen : public Screen {
public:
    /// @brief Creates the start screen, observes shared state, and lays out its controls.
    explicit StartScreen(const GameState& gameState);

    /// @brief Handles clicks and keyboard activation while no overlay is visible.
    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;

    /// @brief Updates any active modal overlay.
    void update(float deltaTime) override;

    /// @brief Draws the background, buttons, and modal overlay.
    void draw(sf::RenderWindow& target) override;

private:
    /// @brief Positions all visual elements for the current fixed window size.
    void configureLayout();

    /// @brief Scales the background image to fill the fixed window.
    void configureBackground();

    /// @brief Positions and styles the settings button and gear icon.
    void configureSettingsButton();

    /// @brief Positions the saved coin counter near the top-right controls.
    void configureCoinCounter();

    /// @brief Positions and styles the quit button icon.
    void configureQuitButtonStyle();

    /// @brief Positions and styles the Start Game and Shop buttons.
    void configureMainButtons();

    /// @brief Styles one menu button.
    void configureMenuButton(sf::RectangleShape& button);

    /// @brief Positions and styles the text labels for the main buttons.
    void configureButtonText();

    /// @brief Handles a left mouse click in screen coordinates.
    void handleClick(sf::Vector2f position);

    /// @brief Updates hover state from the current mouse position.
    void updateHover(sf::Vector2f position);

    /// @brief Returns the button under a point, or None.
    StartScreenButton buttonAt(sf::Vector2f position) const;

    /// @brief Draws the settings button and gear icon.
    void drawSettingsButton(sf::RenderWindow& target) const;

    /// @brief Draws the quit button icon.
    void drawQuitButton(sf::RenderWindow& target) const;

    /// @brief Draws the current coin total beside the top-right quit button.
    void drawCoinCounter(sf::RenderWindow& target);

    /// @brief Positions the quit button against the current render target's top-right corner.
    void updateQuitButtonForTarget(const sf::RenderWindow& target) const;

    /// @brief Draws one text button.
    void drawTextButton(sf::RenderWindow& target,
                        sf::RenderStates states,
                        const sf::RectangleShape& button,
                        const sf::Text& text,
                        StartScreenButton buttonType) const;

    sf::Sprite m_backgroundSprite;
    sf::RectangleShape m_settingsButton;
    sf::Sprite m_settingsSprite;
    Coin m_coin;
    mutable sf::RectangleShape m_quitButton;
    mutable sf::Sprite m_quitSprite;
    sf::RectangleShape m_startButton;
    sf::RectangleShape m_shopButton;
    sf::Text m_startText;
    sf::Text m_shopText;

    const GameState& m_gameState;
    StartScreenButton m_hoveredButton = StartScreenButton::None;
};
