#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

/// @brief Event data derived from the window before dispatching to a screen.
struct ScreenEventContext {
    sf::Vector2f mousePosition{};
    sf::FloatRect viewBounds{};
};
