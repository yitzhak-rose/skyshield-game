#include "Screens/Components/ScrollableTextOverlay.h"

#include "App/Constants.h"
#include "App/ScreenConstants.h"
#include "Resources/ResourceManager.h"
#include "Screens/Core/Screen.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <sstream>
#include <utility>

namespace {
float panelLeft() {
    return WindowConfig::Width / 2.f - ScrollableOverlayConfig::PanelWidth / 2.f;
}

float panelTop() {
    return WindowConfig::Height / 2.f - ScrollableOverlayConfig::PanelHeight / 2.f;
}

float textLeft() {
    return panelLeft() + 80.f;
}

float textTop() {
    return panelTop() + 118.f;
}

bool isHeadingLine(const std::string& line) {
    return line == "Game Credits" ||
        line == "Sounds" ||
        line == "Assets";
}

bool isSubheadingLine(const std::string& line) {
    return line == "Story" ||
        line == "Design" ||
        line == "Development" ||
        line == "YouTube" ||
        line == "Pixabay";
}
}

ScrollableTextOverlay::ScrollableTextOverlay(std::string title, std::string body)
    : m_title(std::move(title))
    , m_backgroundDim({ WindowConfig::Width, WindowConfig::Height })
    , m_dialogPanel({ ScrollableOverlayConfig::PanelWidth, ScrollableOverlayConfig::PanelHeight })
    , m_font(ResourceManager::instance().getFont(Fonts::StencilForce)) {
    configureLayout();
    wrapBodyText(body);
}

void ScrollableTextOverlay::configureLayout() {
    m_backgroundDim.setFillColor(OverlayViewConfig::backgroundDimColor);
    Screen::styleRectangle(
        m_dialogPanel,
        OverlayViewConfig::PanelColor,
        OverlayViewConfig::PanelOutlineColor,
        OverlayViewConfig::PanelOutlineThickness);
    m_dialogPanel.setOrigin({
        ScrollableOverlayConfig::PanelWidth / 2.f,
        ScrollableOverlayConfig::PanelHeight / 2.f
        });
    m_dialogPanel.setPosition({ WindowConfig::Width / 2.f, WindowConfig::Height / 2.f });
}

void ScrollableTextOverlay::wrapBodyText(const std::string& body) {
    std::istringstream stream(body);
    std::string paragraph;
    while (std::getline(stream, paragraph)) {
        if (paragraph.empty()) {
            m_lines.emplace_back();
            continue;
        }

        std::vector<std::string> wrapped = TextUtils::wrapTextLines(
            m_font,
            paragraph,
            ScrollableOverlayConfig::BodyTextSize,
            ScrollableOverlayConfig::TextWidth);
        m_lines.insert(m_lines.end(), wrapped.begin(), wrapped.end());
    }
}

void ScrollableTextOverlay::handleEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (Screen::isMouseMoved(event))
        m_closeHovered = GeometryUtils::contains(closeButtonBounds(), context.mousePosition);

    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>())
        scrollBy(-wheel->delta * ScrollableOverlayConfig::ScrollStep);

    if (Screen::isLeftMousePressed(event) &&
        GeometryUtils::contains(closeButtonBounds(), context.mousePosition)) {
        m_dismissed = true;
    }

    if (Screen::isKeyPressed(event, sf::Keyboard::Key::Escape) ||
        Screen::isAnyKeyPressed(event, { sf::Keyboard::Key::Enter, sf::Keyboard::Key::Space })) {
        m_dismissed = true;
    } else if (Screen::isKeyPressed(event, sf::Keyboard::Key::Up)) {
        scrollBy(-ScrollableOverlayConfig::ScrollStep);
    } else if (Screen::isKeyPressed(event, sf::Keyboard::Key::Down)) {
        scrollBy(ScrollableOverlayConfig::ScrollStep);
    }
}

void ScrollableTextOverlay::scrollBy(float amount) {
    m_scrollOffset = std::clamp(m_scrollOffset + amount, 0.f, maxScroll());
}

float ScrollableTextOverlay::maxScroll() const {
    const float lineHeight = static_cast<float>(ScrollableOverlayConfig::BodyTextSize) + 10.f;
    const float contentHeight = static_cast<float>(m_lines.size()) * lineHeight;
    return std::max(0.f, contentHeight - ScrollableOverlayConfig::TextHeight);
}

sf::FloatRect ScrollableTextOverlay::closeButtonBounds() const {
    return {
        { WindowConfig::Width / 2.f - ScrollableOverlayConfig::CloseButtonWidth / 2.f,
          panelTop() + ScrollableOverlayConfig::PanelHeight - 76.f },
        { ScrollableOverlayConfig::CloseButtonWidth, ScrollableOverlayConfig::CloseButtonHeight }
    };
}

void ScrollableTextOverlay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_backgroundDim, states);
    target.draw(m_dialogPanel, states);

    target.draw(Screen::makeCenteredText(
        m_font,
        m_title,
        ScrollableOverlayConfig::TitleTextSize,
        { WindowConfig::Width / 2.f, panelTop() + 56.f },
        OverlayViewConfig::TitleColor), states);

    const float lineHeight = static_cast<float>(ScrollableOverlayConfig::BodyTextSize) + 10.f;
    const float visibleTop = textTop();
    const float visibleBottom = textTop() + ScrollableOverlayConfig::TextHeight;

    for (std::size_t i = 0; i < m_lines.size(); ++i) {
        const float y = textTop() + static_cast<float>(i) * lineHeight - m_scrollOffset;
        if (y < visibleTop - lineHeight || y > visibleBottom)
            continue;

        if (m_lines[i].empty())
            continue;

        const bool heading = isHeadingLine(m_lines[i]);
        const bool subheading = isSubheadingLine(m_lines[i]);
        const unsigned int size = heading
            ? ScrollableOverlayConfig::HeadingTextSize
            : (subheading ? ScrollableOverlayConfig::SubheadingTextSize : ScrollableOverlayConfig::BodyTextSize);
        const sf::Color color = heading
            ? OverlayViewConfig::TitleColor
            : (subheading ? HelpScreenConfig::AccentColor : OverlayViewConfig::BodyColor);

        target.draw(Screen::makeCenteredText(
            m_font,
            m_lines[i],
            size,
            { WindowConfig::Width / 2.f, y },
            color), states);
    }

    if (maxScroll() > 0.f) {
        sf::RectangleShape track({ 5.f, ScrollableOverlayConfig::TextHeight });
        track.setPosition({
            textLeft() + ScrollableOverlayConfig::TextWidth + 18.f,
            textTop()
            });
        track.setFillColor(OverlayViewConfig::DefaultButtonColor);
        target.draw(track, states);

        const float thumbHeight = std::max(40.f,
            ScrollableOverlayConfig::TextHeight * (ScrollableOverlayConfig::TextHeight /
            (ScrollableOverlayConfig::TextHeight + maxScroll())));
        const float thumbTravel = ScrollableOverlayConfig::TextHeight - thumbHeight;
        sf::RectangleShape thumb({ 5.f, thumbHeight });
        thumb.setPosition({
            track.getPosition().x,
            track.getPosition().y + thumbTravel * (m_scrollOffset / maxScroll())
            });
        thumb.setFillColor(OverlayViewConfig::PanelOutlineColor);
        target.draw(thumb, states);
    }

    const sf::FloatRect closeBounds = closeButtonBounds();
    sf::RectangleShape closeButton(closeBounds.size);
    closeButton.setPosition(closeBounds.position);
    Screen::styleRectangle(
        closeButton,
        m_closeHovered ? OverlayViewConfig::PrimaryButtonColor : OverlayViewConfig::DefaultButtonColor,
        OverlayViewConfig::ButtonOutlineColor,
        OverlayViewConfig::ButtonOutlineThickness);
    target.draw(closeButton, states);
    target.draw(Screen::makeCenteredText(
        m_font,
        "Close",
        ScrollableOverlayConfig::ButtonTextSize,
        Screen::centerOf(closeBounds),
        OverlayViewConfig::ButtonTextColor), states);
}
