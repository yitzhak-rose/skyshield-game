#include "Screens/Core/Screen.h"

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

void Screen::requestCommand(ScreenCommand command) {
    if (m_command == ScreenCommand::None)
        m_command = command;
}

void Screen::showOverlay(std::unique_ptr<OverlayView> overlay) {
    m_overlay = std::move(overlay);
}
