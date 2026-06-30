#pragma once

#include "Structs/TextButtonColors.h"
#include "Structs/TextButtonMetrics.h"

#include <SFML/Graphics/Color.hpp>

/// @brief Creates reusable TextButton color and metric presets for app screens.
class ButtonStyles {
public:
    static TextButtonColors startMenuColors();
    static TextButtonMetrics startMenuMetrics();

    static TextButtonColors shopColors(sf::Color fill);
    static TextButtonMetrics shopMetrics(unsigned int size, unsigned int minSize);

    static TextButtonColors settingsColors(sf::Color fill, sf::Color hoverFill);
    static TextButtonMetrics settingsMetrics(unsigned int size, unsigned int minSize);

    static TextButtonColors overlayColors(sf::Color fill);
    static TextButtonMetrics overlayMetrics(unsigned int size, unsigned int minSize);

    static TextButtonColors overlayHoverColors(sf::Color fill, sf::Color hoverFill);

    static TextButtonColors disabledOverlayColors(sf::Color fill, sf::Color disabledFill);

private:
    ButtonStyles() = default;
};
