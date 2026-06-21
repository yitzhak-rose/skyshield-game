#include "Screens/Screen.h"

#include <optional>
#include <utility>

Screen::Screen() = default;

Screen::~Screen() = default;

void Screen::handleEvents(sf::RenderWindow& window) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
        else if (!handleOverlayEvent(*event))
            handleEvent(*event);

        clearDismissedOverlay();
    }
}

void Screen::showOverlay(std::unique_ptr<OverlayView> overlay) {
    m_overlay = std::move(overlay);
}

bool Screen::handleOverlayEvent(const sf::Event& event) {
    if (!hasOverlay())
        return false;

    m_overlay->handleEvent(event);
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

void Screen::drawOverlay(sf::RenderWindow& window) const {
    if (!hasOverlay())
        return;

    const sf::View previousView = window.getView();
    window.setView(window.getDefaultView());
    window.draw(*m_overlay);
    window.setView(previousView);
}

void Screen::clearDismissedOverlay() {
    if (hasOverlay() && m_overlay->isDismissed())
        m_overlay.reset();
}
