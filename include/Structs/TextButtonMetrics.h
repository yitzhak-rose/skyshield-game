#pragma once

#include <SFML/System/Vector2.hpp>

/// @brief Stores TextButton sizing, padding, outline, and text-fitting settings.
struct TextButtonMetrics {
    unsigned int textSize = 28;
    unsigned int minTextSize = 16;
    float horizontalPadding = 18.f;
    float verticalPadding = 8.f;
    float outlineThickness = 2.f;
    float selectedOutlineThickness = 4.f;
    sf::Vector2f textOffset = { 0.f, 0.f };
    bool expandToFitText = false;
};
