#include "Screens/Components/TextButton.h"

#include "Utilities/Utils.h"

#include <algorithm>
#include <utility>

TextButton::TextButton(const sf::Font& font, std::string label)
    : TextButton(font, std::move(label), { { 0.f, 0.f }, { 0.f, 0.f } }) {
}

TextButton::TextButton(const sf::Font& font, std::string label, sf::FloatRect bounds)
    : m_font(font)
    , m_label(std::move(label))
    , m_requestedBounds(bounds) {
}

void TextButton::setLabel(std::string label) {
    m_label = std::move(label);
}

void TextButton::setBounds(sf::FloatRect bounds) {
    m_requestedBounds = bounds;
}

void TextButton::setAvailableBounds(sf::FloatRect bounds) {
    m_availableBounds = bounds;
    m_hasAvailableBounds = true;
}

void TextButton::clearAvailableBounds() {
    m_hasAvailableBounds = false;
}

void TextButton::setColors(const TextButtonColors& colors) {
    m_colors = colors;
}

void TextButton::setMetrics(const TextButtonMetrics& metrics) {
    m_metrics = metrics;
}

void TextButton::setCallback(std::function<void()> callback) {
    m_callback = std::move(callback);
}

void TextButton::setHovered(bool hovered) {
    m_hovered = hovered;
}

void TextButton::setPressed(bool pressed) {
    m_pressed = pressed;
}

void TextButton::setSelected(bool selected) {
    m_selected = selected;
}

void TextButton::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool TextButton::updateHover(sf::Vector2f position) {
    m_hovered = GeometryUtils::contains(fittedBounds(), position);
    return m_hovered;
}

bool TextButton::click(sf::Vector2f position) const {
    if (!m_enabled || !GeometryUtils::contains(fittedBounds(), position))
        return false;

    activate();
    return true;
}

void TextButton::activate() const {
    if (m_enabled && m_callback)
        m_callback();
}

sf::FloatRect TextButton::bounds() const {
    return fittedBounds();
}

void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    const sf::FloatRect buttonBounds = fittedBounds();

    if (m_colors.hasBackground) {
        sf::RectangleShape background(buttonBounds.size);
        background.setPosition(buttonBounds.position);
        background.setFillColor(m_colors.background);
        target.draw(background, states);
    }

    sf::RectangleShape button(buttonBounds.size);
    button.setPosition(buttonBounds.position);
    button.setFillColor(fillColor());
    button.setOutlineColor(m_colors.border);
    button.setOutlineThickness(m_selected
        ? m_metrics.selectedOutlineThickness
        : m_metrics.outlineThickness);
    target.draw(button, states);

    target.draw(fittedText(buttonBounds), states);
}

sf::FloatRect TextButton::fittedBounds() const {
    sf::FloatRect bounds = m_requestedBounds;
    if (!m_metrics.expandToFitText || !m_hasAvailableBounds)
        return bounds;

    const float preferredWidth = preferredTextWidth() + m_metrics.horizontalPadding * 2.f;
    if (preferredWidth <= bounds.size.x)
        return bounds;

    const float expandedWidth = std::min(preferredWidth, m_availableBounds.size.x);
    if (expandedWidth <= bounds.size.x)
        return bounds;

    bounds.size.x = expandedWidth;

    const float requestedCenterX = m_requestedBounds.position.x + m_requestedBounds.size.x / 2.f;
    bounds.position.x = requestedCenterX - bounds.size.x / 2.f;
    bounds.position.x = std::clamp(
        bounds.position.x,
        m_availableBounds.position.x,
        m_availableBounds.position.x + m_availableBounds.size.x - bounds.size.x);

    return bounds;
}

sf::Text TextButton::fittedText(sf::FloatRect bounds) const {
    sf::Text text(m_font, m_label, m_metrics.textSize);
    text.setFillColor(m_colors.text);

    const float maxWidth = std::max(0.f, bounds.size.x - m_metrics.horizontalPadding * 2.f);
    const float maxHeight = std::max(0.f, bounds.size.y - m_metrics.verticalPadding * 2.f);
    while ((text.getLocalBounds().size.x > maxWidth || text.getLocalBounds().size.y > maxHeight) &&
           text.getCharacterSize() > m_metrics.minTextSize) {
        text.setCharacterSize(text.getCharacterSize() - 1u);
    }

    GraphicsUtils::centerOrigin(text);
    text.setPosition(GeometryUtils::centerOf(bounds) + m_metrics.textOffset);
    return text;
}

sf::Color TextButton::fillColor() const {
    if (!m_enabled && m_colors.hasDisabledFill)
        return m_colors.disabledFill;
    if (m_pressed && m_colors.hasPressedFill)
        return m_colors.pressedFill;
    if (m_hovered && m_colors.hasHoverFill)
        return m_colors.hoverFill;
    return m_colors.fill;
}

float TextButton::preferredTextWidth() const {
    sf::Text text(m_font, m_label, m_metrics.textSize);
    return text.getLocalBounds().size.x;
}
