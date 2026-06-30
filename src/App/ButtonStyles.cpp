#include "App/ButtonStyles.h"

#include "App/ScreenConstants.h"

TextButtonColors ButtonStyles::startMenuColors() {
    TextButtonColors colors;
    colors.fill = StartScreenConfig::ButtonColor;
    colors.border = StartScreenConfig::ButtonOutlineColor;
    colors.text = StartScreenConfig::ButtonTextColor;
    colors.hoverFill = StartScreenConfig::ButtonHoverColor;
    colors.hasHoverFill = true;
    return colors;
}

TextButtonMetrics ButtonStyles::startMenuMetrics() {
    TextButtonMetrics metrics;
    metrics.textSize = StartScreenConfig::ButtonTextSize;
    metrics.minTextSize = 20;
    metrics.outlineThickness = StartScreenConfig::ButtonOutlineThickness;
    return metrics;
}

TextButtonColors ButtonStyles::shopColors(sf::Color fill) {
    TextButtonColors colors;
    colors.fill = fill;
    colors.border = ShopScreenConfig::OutlineColor;
    colors.text = ShopScreenConfig::TextColor;
    return colors;
}

TextButtonMetrics ButtonStyles::shopMetrics(unsigned int size, unsigned int minSize) {
    TextButtonMetrics metrics;
    metrics.textSize = size;
    metrics.minTextSize = minSize;
    metrics.horizontalPadding = 14.f;
    metrics.outlineThickness = 2.f;
    return metrics;
}

TextButtonColors ButtonStyles::settingsColors(sf::Color fill, sf::Color hoverFill) {
    TextButtonColors colors;
    colors.fill = fill;
    colors.border = SettingsScreenConfig::PanelOutlineColor;
    colors.text = SettingsScreenConfig::TextColor;
    colors.hoverFill = hoverFill;
    colors.hasHoverFill = true;
    return colors;
}

TextButtonMetrics ButtonStyles::settingsMetrics(unsigned int size, unsigned int minSize) {
    TextButtonMetrics metrics;
    metrics.textSize = size;
    metrics.minTextSize = minSize;
    metrics.outlineThickness = SettingsScreenConfig::ButtonOutlineThickness;
    metrics.selectedOutlineThickness = SettingsScreenConfig::ActiveButtonOutlineThickness;
    metrics.expandToFitText = true;
    return metrics;
}

TextButtonColors ButtonStyles::overlayColors(sf::Color fill) {
    TextButtonColors colors;
    colors.fill = fill;
    colors.border = OverlayViewConfig::ButtonOutlineColor;
    colors.text = OverlayViewConfig::ButtonTextColor;
    return colors;
}

TextButtonMetrics ButtonStyles::overlayMetrics(unsigned int size, unsigned int minSize) {
    TextButtonMetrics metrics;
    metrics.textSize = size;
    metrics.minTextSize = minSize;
    metrics.outlineThickness = OverlayViewConfig::ButtonOutlineThickness;
    metrics.selectedOutlineThickness = OverlayViewConfig::SelectedButtonOutlineThickness;
    return metrics;
}

TextButtonColors ButtonStyles::overlayHoverColors(sf::Color fill, sf::Color hoverFill) {
    TextButtonColors colors = overlayColors(fill);
    colors.hoverFill = hoverFill;
    colors.hasHoverFill = true;
    return colors;
}

TextButtonColors ButtonStyles::disabledOverlayColors(sf::Color fill, sf::Color disabledFill) {
    TextButtonColors colors = overlayColors(fill);
    colors.disabledFill = disabledFill;
    colors.hasDisabledFill = true;
    return colors;
}
