#pragma once

#include "Structs/ScreenEventContext.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

/// @brief Modal overlay for long scrollable text such as credits/attribution.
class ScrollableTextOverlay : public sf::Drawable, public sf::Transformable {
public:
    ScrollableTextOverlay(std::string title, std::string body);

    void handleEvent(const sf::Event& event, const ScreenEventContext& context);
    bool isDismissed() const { return m_dismissed; }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void configureLayout();
    void wrapBodyText(const std::string& body);
    void scrollBy(float amount);
    float maxScroll() const;
    sf::FloatRect closeButtonBounds() const;

    std::string m_title;
    std::vector<std::string> m_lines;
    sf::RectangleShape m_backgroundDim;
    sf::RectangleShape m_dialogPanel;
    const sf::Font& m_font;
    float m_scrollOffset = 0.f;
    bool m_closeHovered = false;
    bool m_dismissed = false;
};
