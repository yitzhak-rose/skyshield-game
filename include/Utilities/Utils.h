#pragma once

#include <SFML/Graphics.hpp>

namespace Utils {
    /// @brief Returns a random float in the range [min, max].
    float randomFloat(float min, float max);
}

namespace GeometryUtils {
    /// @brief Checks whether a point lies inside a rectangle.
    bool contains(sf::FloatRect bounds, sf::Vector2f position);
}

namespace GraphicsUtils {
    /// @brief Sets a sprite origin to the center of its local bounds.
    void centerOrigin(sf::Sprite& sprite);

    /// @brief Sets a text origin to the center of its local bounds.
    void centerOrigin(sf::Text& text);
}
