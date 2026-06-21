#pragma once

#include "Screens/Screen.h"

#include <SFML/Graphics.hpp>
#include <functional>

/// @brief Opening screen with start, shop, and settings actions.
class StartScreen : public Screen {
public:
    /// @brief Creates the start screen and lays out its controls.
    /// @param quitGame Callback used when the quit button is clicked.
    explicit StartScreen(std::function<void()> quitGame);

    /// @brief Handles clicks and keyboard activation while no overlay is visible.
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    /// @brief Updates any active modal overlay.
    void update(float deltaTime) override;

    /// @brief Draws the background, buttons, and modal overlay.
    void draw(sf::RenderWindow& window) override;

    /// @brief Returns true once Start Game has been chosen.
    bool isFinished() const override { return m_startRequested; }

    /// @brief Reports completion to Game so it can enter stage 0.
    LevelResult getResult() const override;

private:
    enum class Button {
        None,
        Start,
        Shop,
        Settings,
        Quit
    };

    /// @brief Positions all visual elements for the current fixed window size.
    void configureLayout();

    /// @brief Scales the background image to fill the fixed window.
    void configureBackground();

    /// @brief Positions and styles the settings button and gear icon.
    void configureSettingsButton();

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
    Button buttonAt(sf::Vector2f position) const;

    /// @brief Shows the shared feature-unavailable dialog.
    void showFeatureUnavailableOverlay();

    /// @brief Draws the settings button and gear icon.
    void drawSettingsButton(sf::RenderTarget& target) const;

    /// @brief Draws the quit button icon.
    void drawQuitButton(sf::RenderTarget& target) const;

    /// @brief Positions the quit button against the current render target's top-right corner.
    void updateQuitButtonForTarget(const sf::RenderTarget& target) const;

    /// @brief Draws one text button.
    void drawTextButton(sf::RenderTarget& target,
                        sf::RenderStates states,
                        const sf::RectangleShape& button,
                        const sf::Text& text,
                        Button buttonType) const;

    sf::Sprite m_backgroundSprite;
    sf::RectangleShape m_settingsButton;
    sf::Sprite m_settingsSprite;
    mutable sf::RectangleShape m_quitButton;
    mutable sf::Sprite m_quitSprite;
    sf::RectangleShape m_startButton;
    sf::RectangleShape m_shopButton;
    sf::Text m_startText;
    sf::Text m_shopText;

    std::function<void()> m_quitGame;

    Button m_hoveredButton = Button::None;
    bool m_startRequested = false;
};
