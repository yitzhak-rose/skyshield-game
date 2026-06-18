#pragma once
#include <SFML/Graphics.hpp>

/// @brief Interface for objects that can draw themselves to the window.
class IDrawable {
public:
    /// @brief Virtual destructor.
    virtual ~IDrawable() = default;

    /// @brief Draws the object to the window.
    virtual void draw(sf::RenderWindow& window) const = 0;
};