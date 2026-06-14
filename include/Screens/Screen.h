#pragma once
#include <SFML/Graphics.hpp>

/// \brief Abstract base class for all game screens.
class Screen {
public:
    /// \brief Default constructor.
    Screen();

    /// \brief Virtual destructor.
    virtual ~Screen() = 0;

    /// \brief Draws the screen content.
    /// \param window The render window.
    virtual void draw(sf::RenderWindow& window) = 0;

    /// \brief Handles input events (mouse clicks, etc.).
    /// \param mousePos The current mouse position in world coordinates.
    virtual void handleInput(sf::Vector2f /*mousePos*/) {}

    /// \brief Updates the screen logic.
    /// \param deltaTime Time elapsed since last frame.
    virtual void update(float /*deltaTime*/) {}
};