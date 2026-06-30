#pragma once

#include "Structs/TextButtonColors.h"
#include "Structs/TextButtonMetrics.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

/// @brief Reusable rectangular text button with hover, pressed, disabled, fitting, and callback support.
class TextButton : public sf::Drawable {
public:
    TextButton(const sf::Font& font, std::string label);
    TextButton(const sf::Font& font, std::string label, sf::FloatRect bounds);

    void setLabel(std::string label);
    void setBounds(sf::FloatRect bounds);
    void setAvailableBounds(sf::FloatRect bounds);
    void clearAvailableBounds();
    void setColors(const TextButtonColors& colors);
    void setMetrics(const TextButtonMetrics& metrics);
    void setCallback(std::function<void()> callback);
    void setHovered(bool hovered);
    void setPressed(bool pressed);
    void setSelected(bool selected);
    void setEnabled(bool enabled);

    bool contains(sf::Vector2f position) const;
    bool updateHover(sf::Vector2f position);
    bool click(sf::Vector2f position) const;
    void activate() const;
    sf::FloatRect bounds() const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::FloatRect fittedBounds() const;
    sf::Text fittedText(sf::FloatRect bounds) const;
    sf::Color fillColor() const;
    float preferredTextWidth() const;
    static sf::Vector2f centerOf(sf::FloatRect bounds);

    const sf::Font& m_font;
    std::string m_label;
    sf::FloatRect m_requestedBounds;
    sf::FloatRect m_availableBounds;
    TextButtonColors m_colors;
    TextButtonMetrics m_metrics;
    std::function<void()> m_callback;
    bool m_hasAvailableBounds = false;
    bool m_hovered = false;
    bool m_pressed = false;
    bool m_selected = false;
    bool m_enabled = true;
};
