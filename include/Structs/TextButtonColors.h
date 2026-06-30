#pragma once

#include <SFML/Graphics/Color.hpp>

/// @brief Stores the colors used by a TextButton in its visual states.
struct TextButtonColors {
    sf::Color fill = sf::Color::Transparent;
    sf::Color border = sf::Color::Transparent;
    sf::Color text = sf::Color::White;
    sf::Color hoverFill = sf::Color::Transparent;
    sf::Color pressedFill = sf::Color::Transparent;
    sf::Color disabledFill = sf::Color::Transparent;
    sf::Color background = sf::Color::Transparent;
    bool hasHoverFill = false;
    bool hasPressedFill = false;
    bool hasDisabledFill = false;
    bool hasBackground = false;
};
