#include "Menubar.h"

MenuBar::MenuBar(float width, float height, GameState& gameState, const sf::Font& font)
    : m_background({width, height}), m_state(gameState), m_font(font) {
}

sf::Text MenuBar::displayText(std::string, int, sf::Vector2f, const sf::Font& font) const {
    return sf::Text(font);
}

void MenuBar::draw(sf::RenderTarget&, sf::RenderStates) const {
}

void MenuBar::drawMenuBarInfo(sf::RenderTarget&, sf::RenderStates) const {
}
