#include "Screens/OverlayView.h"
#include "Gameplay/Constants.h"
#include "Resources/ResourceManager.h"
#include "Screens/Screen.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <utility>

OverlayView::OverlayView(std::string title,
                         std::vector<std::string> bodyLines,
                         std::vector<OverlayAction> actions)
    : m_title(std::move(title))
    , m_bodyLines(std::move(bodyLines))
    , m_actions(std::move(actions))
    , m_scrim({ WindowConfig::Width, WindowConfig::Height })
    , m_panel({ OverlayViewConfig::PanelWidth, OverlayViewConfig::PanelHeight })
    , m_font(ResourceManager::instance().getFont(Fonts::StencilForce)) {
    validateActions();
    configureLayout();
}

void OverlayView::validateActions() const {
    if (m_actions.empty())
        throw std::invalid_argument("OverlayView requires at least one action");

    if (m_actions.size() > OverlayViewConfig::MaxActions)
        throw std::invalid_argument("OverlayView received too many actions");
}

void OverlayView::configureLayout() {
    m_scrim.setFillColor(OverlayViewConfig::ScrimColor);

    m_panel.setFillColor(OverlayViewConfig::PanelColor);
    m_panel.setOutlineColor(OverlayViewConfig::PanelOutlineColor);
    m_panel.setOutlineThickness(OverlayViewConfig::PanelOutlineThickness);
    m_panel.setOrigin({ OverlayViewConfig::PanelWidth / 2.f,
                        OverlayViewConfig::PanelHeight / 2.f });
    m_panel.setPosition({ WindowConfig::Width / 2.f, WindowConfig::Height / 2.f });

    const float totalButtonWidth =
        static_cast<float>(m_actions.size()) * OverlayViewConfig::ButtonWidth +
        static_cast<float>(m_actions.size() - 1) * OverlayViewConfig::ButtonGap;
    const float startX = WindowConfig::Width / 2.f - totalButtonWidth / 2.f;
    const float y = WindowConfig::Height / 2.f + OverlayViewConfig::ButtonTopOffset;

    m_buttonBounds.clear();
    for (std::size_t i = 0; i < m_actions.size(); ++i) {
        const float x = startX + static_cast<float>(i) *
            (OverlayViewConfig::ButtonWidth + OverlayViewConfig::ButtonGap);
        m_buttonBounds.emplace_back(
            sf::Vector2f{ x, y },
            sf::Vector2f{ OverlayViewConfig::ButtonWidth, OverlayViewConfig::ButtonHeight });
    }

    m_selectedAction = m_actions.size() - 1;
}

void OverlayView::handleEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (event.is<sf::Event::MouseMoved>()) {
        updateSelectionFromMouse(context.mousePosition);
    }

    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button != sf::Mouse::Button::Left)
            return;

        const int actionIndex = buttonIndexAt(context.mousePosition);
        if (actionIndex >= 0)
            activateAction(static_cast<std::size_t>(actionIndex));
    }

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Left)
            moveSelectionLeft();
        else if (key->code == sf::Keyboard::Key::Right)
            moveSelectionRight();
        else if (key->code == sf::Keyboard::Key::Enter ||
                 key->code == sf::Keyboard::Key::Space)
            activateAction(m_selectedAction);
    }
}

void OverlayView::updateSelectionFromMouse(sf::Vector2f mousePosition) {
    const int actionIndex = buttonIndexAt(mousePosition);
    if (actionIndex >= 0)
        m_selectedAction = static_cast<std::size_t>(actionIndex);
}

int OverlayView::buttonIndexAt(sf::Vector2f position) const {
    for (std::size_t i = 0; i < m_buttonBounds.size(); ++i) {
        const sf::FloatRect& bounds = m_buttonBounds[i];
        const bool insideX = position.x >= bounds.position.x &&
            position.x <= bounds.position.x + bounds.size.x;
        const bool insideY = position.y >= bounds.position.y &&
            position.y <= bounds.position.y + bounds.size.y;

        if (insideX && insideY)
            return static_cast<int>(i);
    }

    return -1;
}

void OverlayView::activateAction(std::size_t actionIndex) {
    if (actionIndex >= m_actions.size())
        return;
    if (!m_actions[actionIndex].enabled)
        return;

    auto action = m_actions[actionIndex].action;
    m_dismissed = true;

    if (action)
        action();
}

void OverlayView::moveSelectionLeft() {
    if (m_selectedAction > 0)
        --m_selectedAction;
}

void OverlayView::moveSelectionRight() {
    if (m_selectedAction + 1 < m_actions.size())
        ++m_selectedAction;
}

void OverlayView::update(float /*dt*/) {
}

void OverlayView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_scrim, states);
    target.draw(m_panel, states);
    drawText(target, states);
    drawButtons(target, states);
}

void OverlayView::drawText(sf::RenderTarget& target, sf::RenderStates states) const {
    const sf::Vector2f center{ WindowConfig::Width / 2.f, WindowConfig::Height / 2.f };

    target.draw(makeCenteredText(m_title,
                                 OverlayViewConfig::TitleTextSize,
                                 { center.x, center.y + OverlayViewConfig::TitleTopOffset },
                                 OverlayViewConfig::TitleColor),
                states);

    float bodyY = center.y + OverlayViewConfig::BodyTopOffset;
    for (const auto& line : m_bodyLines) {
        target.draw(makeCenteredText(line,
                                     OverlayViewConfig::BodyTextSize,
                                     { center.x, bodyY },
                                     OverlayViewConfig::BodyColor),
                    states);
        bodyY += OverlayViewConfig::BodyLineSpacing;
    }
}

void OverlayView::drawButtons(sf::RenderTarget& target, sf::RenderStates states) const {
    for (std::size_t i = 0; i < m_actions.size(); ++i) {
        const sf::FloatRect& bounds = m_buttonBounds[i];
        sf::RectangleShape button(bounds.size);
        button.setPosition(bounds.position);
        button.setFillColor(buttonColor(m_actions[i], i == m_selectedAction));
        button.setOutlineColor(OverlayViewConfig::ButtonOutlineColor);
        button.setOutlineThickness(i == m_selectedAction
            ? OverlayViewConfig::SelectedButtonOutlineThickness
            : OverlayViewConfig::ButtonOutlineThickness);

        target.draw(button, states);
        target.draw(makeCenteredText(m_actions[i].label,
                                     OverlayViewConfig::ButtonTextSize,
                                     { bounds.position.x + bounds.size.x / 2.f,
                                       bounds.position.y + bounds.size.y / 2.f - 2.f },
                                     OverlayViewConfig::ButtonTextColor),
                    states);
    }
}

sf::Color OverlayView::buttonColor(const OverlayAction& action, bool selected) const {
    sf::Color color = OverlayViewConfig::DefaultButtonColor;

    if (!action.enabled)
        return OverlayViewConfig::DisabledButtonColor;

    if (action.type == OverlayActionType::Primary)
        color = OverlayViewConfig::PrimaryButtonColor;
    else if (action.type == OverlayActionType::Danger)
        color = OverlayViewConfig::DangerButtonColor;

    if (selected) {
        color.r = static_cast<std::uint8_t>(std::min(255, color.r + OverlayViewConfig::SelectedColorBoost));
        color.g = static_cast<std::uint8_t>(std::min(255, color.g + OverlayViewConfig::SelectedColorBoost));
        color.b = static_cast<std::uint8_t>(std::min(255, color.b + OverlayViewConfig::SelectedColorBoost));
    }

    return color;
}

sf::Text OverlayView::makeCenteredText(const std::string& value,
                                       unsigned int size,
                                       sf::Vector2f position,
                                       sf::Color color) const {
    sf::Text text(m_font, value, size);
    text.setFillColor(color);
    GraphicsUtils::centerOrigin(text);
    text.setPosition(position);
    return text;
}
