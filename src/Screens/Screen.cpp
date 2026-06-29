#include "Screens/Screen.h"

#include "Utilities/Utils.h"

#include <algorithm>
#include <utility>

Screen::Screen() = default;

Screen::~Screen() = default;

void Screen::handleEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (!handleOverlayEvent(event, context))
        handleScreenEvent(event, context);

    clearDismissedOverlay();
}

bool Screen::isMouseMoved(const sf::Event& event) {
    return event.is<sf::Event::MouseMoved>();
}

bool Screen::isLeftMousePressed(const sf::Event& event) {
    const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>();
    return mousePress && mousePress->button == sf::Mouse::Button::Left;
}

bool Screen::isKeyPressed(const sf::Event& event, sf::Keyboard::Key key) {
    const auto* keyPress = event.getIf<sf::Event::KeyPressed>();
    return keyPress && keyPress->code == key;
}

bool Screen::isAnyKeyPressed(const sf::Event& event, std::initializer_list<sf::Keyboard::Key> keys) {
    const auto* keyPress = event.getIf<sf::Event::KeyPressed>();
    if (!keyPress)
        return false;

    return std::find(keys.begin(), keys.end(), keyPress->code) != keys.end();
}

bool Screen::handleOverlayEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (!hasOverlay())
        return false;

    m_overlay->handleEvent(event, context);
    clearDismissedOverlay();
    return true;
}

bool Screen::updateOverlay(float deltaTime) {
    if (!hasOverlay())
        return false;

    m_overlay->update(deltaTime);
    clearDismissedOverlay();
    return true;
}

void Screen::drawOverlay(sf::RenderWindow& target) const {
    if (!hasOverlay())
        return;

    target.draw(*m_overlay);
}

void Screen::clearDismissedOverlay() {
    if (hasOverlay() && m_overlay->isDismissed())
        m_overlay.reset();
}

void Screen::showOverlay(std::unique_ptr<OverlayView> overlay) {
    m_overlay = std::move(overlay);
}

bool Screen::contains(sf::FloatRect bounds, sf::Vector2f position) {
    return position.x >= bounds.position.x &&
        position.x <= bounds.position.x + bounds.size.x &&
        position.y >= bounds.position.y &&
        position.y <= bounds.position.y + bounds.size.y;
}

sf::Vector2f Screen::centerOf(sf::FloatRect bounds) {
    return {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
}

sf::Vector2f Screen::centerOf(const sf::RectangleShape& shape) {
    return centerOf(shape.getGlobalBounds());
}

void Screen::fitSpriteTo(sf::Sprite& sprite, sf::FloatRect bounds) {
    const sf::IntRect textureRect = sprite.getTextureRect();
    const sf::Vector2i size = textureRect.size;
    const float scale = std::min(
        bounds.size.x / static_cast<float>(size.x),
        bounds.size.y / static_cast<float>(size.y));
    sprite.setScale({ scale, scale });
    GraphicsUtils::centerOrigin(sprite);
    sprite.setPosition(centerOf(bounds));
}

void Screen::styleRectangle(sf::RectangleShape& shape,
                            sf::Color fill,
                            sf::Color outline,
                            float outlineThickness) {
    shape.setFillColor(fill);
    shape.setOutlineColor(outline);
    shape.setOutlineThickness(outlineThickness);
}

sf::Text Screen::makeText(const sf::Font& font,
                          const std::string& value,
                          unsigned int size,
                          sf::Vector2f position,
                          sf::Color color) {
    sf::Text text(font, value, size);
    text.setFillColor(color);
    text.setPosition(position);
    return text;
}

sf::Text Screen::makeCenteredText(const sf::Font& font,
                                  const std::string& value,
                                  unsigned int size,
                                  sf::Vector2f position,
                                  sf::Color color) {
    sf::Text text = makeText(font, value, size, position, color);
    GraphicsUtils::centerOrigin(text);
    return text;
}
