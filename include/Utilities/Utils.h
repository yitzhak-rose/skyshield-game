#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

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

namespace TextUtils {
    /// @brief Splits text into lines that fit within maxWidth, optionally limiting line count.
    std::vector<std::string> wrapTextLines(const sf::Font& font,
                                           const std::string& text,
                                           unsigned int size,
                                           float maxWidth,
                                           std::size_t maxLines = 0);

    /// @brief Draws several lines of text with fixed vertical spacing.
    void drawMultiline(sf::RenderTarget& target,
                       const sf::Font& font,
                       const std::vector<std::string>& lines,
                       unsigned int size,
                       sf::Vector2f position,
                       sf::Color color,
                       float lineSpacing);
}
