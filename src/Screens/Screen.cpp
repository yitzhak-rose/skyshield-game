#include "Screens/Screen.h"

#include <utility>

Screen::Screen() = default;

Screen::~Screen() = default;

void Screen::handleEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (!handleOverlayEvent(event, context))
        handleScreenEvent(event, context);

    clearDismissedOverlay();
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
