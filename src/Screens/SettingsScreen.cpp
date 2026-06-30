#include "Screens/SettingsScreen.h"

#include "App/ButtonStyles.h"
#include "App/Constants.h"
#include "App/ScreenConstants.h"
#include "Resources/ResourceManager.h"
#include "Screens/Components/TextButton.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

SettingsScreen::SettingsScreen(const GameState& gameState)
    : m_gameState(gameState)
    , m_background({ WindowConfig::Width, WindowConfig::Height })
    , m_panel({ SettingsScreenConfig::PanelWidth, SettingsScreenConfig::PanelHeight })
    , m_backSprite(ResourceManager::instance().getImage(GameObjects::ChevronBack))
    , m_volumeLowSprite(ResourceManager::instance().getImage(GameObjects::VolumeLow))
    , m_volumeHighSprite(ResourceManager::instance().getImage(GameObjects::VolumeHigh))
    , m_font(ResourceManager::instance().getFont(Fonts::StencilForce)) {
    configureLayout();
}

void SettingsScreen::configureLayout() {
    m_background.setFillColor(SettingsScreenConfig::BackgroundColor);
    m_panel.setPosition({ SettingsScreenConfig::PanelX, SettingsScreenConfig::PanelY });
    styleRectangle(
        m_panel,
        SettingsScreenConfig::PanelColor,
        SettingsScreenConfig::PanelOutlineColor,
        SettingsScreenConfig::ButtonOutlineThickness);

    fitSpriteTo(m_backSprite, backButtonBounds());
    fitSpriteTo(m_volumeLowSprite, {
        { SettingsScreenConfig::SliderX -
              SettingsScreenConfig::VolumeIconSize -
              SettingsScreenConfig::VolumeIconGap,
          SettingsScreenConfig::SliderY - SettingsScreenConfig::VolumeIconSize / 2.f },
        { SettingsScreenConfig::VolumeIconSize, SettingsScreenConfig::VolumeIconSize }
    });
    fitSpriteTo(m_volumeHighSprite, {
        { SettingsScreenConfig::SliderX +
              SettingsScreenConfig::SliderWidth +
              SettingsScreenConfig::VolumeIconGap,
          SettingsScreenConfig::SliderY - SettingsScreenConfig::VolumeIconSize / 2.f },
        { SettingsScreenConfig::VolumeIconSize, SettingsScreenConfig::VolumeIconSize }
    });
}

void SettingsScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (m_attributionOverlay) {
        m_attributionOverlay->handleEvent(event, context);
        if (m_attributionOverlay->isDismissed())
            m_attributionOverlay.reset();
        return;
    }

    if (isMouseMoved(event)) {
        updateHover(context.mousePosition);
        handleDrag(context.mousePosition);
    }

    if (isLeftMousePressed(event))
        handleClick(context.mousePosition);

    if (event.is<sf::Event::MouseButtonReleased>())
        m_draggingSlider = false;

    if (isKeyPressed(event, sf::Keyboard::Key::Escape))
        requestAction(SettingsAction::ReturnToStart);
}

void SettingsScreen::update(float deltaTime) {
    updateOverlay(deltaTime);
}

void SettingsScreen::handleClick(sf::Vector2f position) {
    switch (controlAt(position)) {
        case SettingsControl::Back:
            requestAction(SettingsAction::ReturnToStart);
            break;
        case SettingsControl::Mute:
            m_draggingSlider = false;
            requestAction(SettingsAction::Mute);
            break;
        case SettingsControl::Unmute:
            requestAction(SettingsAction::Unmute);
            break;
        case SettingsControl::Slider:
            if (!m_gameState.isMuted()) {
                m_draggingSlider = true;
                updateVolumeFromPosition(position.x);
            }
            break;
        case SettingsControl::Help:
            requestAction(SettingsAction::OpenHelp);
            break;
        case SettingsControl::Attribution:
            showAttribution();
            break;
        case SettingsControl::Reset:
            showResetConfirmation();
            break;
        case SettingsControl::None:
            break;
    }
}

void SettingsScreen::handleDrag(sf::Vector2f position) {
    if (!m_draggingSlider || m_gameState.isMuted())
        return;

    updateVolumeFromPosition(position.x);
}

void SettingsScreen::updateHover(sf::Vector2f position) {
    m_hoveredControl = controlAt(position);
}

void SettingsScreen::updateVolumeFromPosition(float x) {
    const sf::FloatRect track = sliderTrackBounds();
    const float ratio = std::clamp((x - track.position.x) / track.size.x, 0.f, 1.f);
    m_requestedVolume = ratio * SettingsScreenConfig::MaxVolume;
    m_hasVolumeChange = true;
}

void SettingsScreen::showResetConfirmation() {
    showOverlay(std::make_unique<OverlayView>(
        "Reset User Data",
        std::vector<std::string>{
            "Resetting will return the app to defaults.",
            "All user-added data will be removed."
        },
        std::vector<OverlayAction>{
            { "Cancel", OverlayActionType::Default, []() {} },
            { "Reset", OverlayActionType::Danger, [this]() { requestAction(SettingsAction::ResetUserData); } }
        }));
}

void SettingsScreen::showAttribution() {
    m_attributionOverlay = std::make_unique<ScrollableTextOverlay>(
        "Attribution",
        SettingsScreenConfig::AttributionText);
}

void SettingsScreen::requestAction(SettingsAction action) {
    if (m_requestedAction == SettingsAction::None)
        m_requestedAction = action;
}

SettingsAction SettingsScreen::consumeAction() {
    const SettingsAction action = m_requestedAction;
    m_requestedAction = SettingsAction::None;
    return action;
}

bool SettingsScreen::consumeVolumeChange(float& volume) {
    if (!m_hasVolumeChange)
        return false;

    volume = m_requestedVolume;
    m_hasVolumeChange = false;
    return true;
}

SettingsControl SettingsScreen::controlAt(sf::Vector2f position) const {
    if (GeometryUtils::contains(backButtonBounds(), position))
        return SettingsControl::Back;
    if (GeometryUtils::contains(audioButtonBounds(SettingsControl::Mute), position))
        return SettingsControl::Mute;
    if (GeometryUtils::contains(audioButtonBounds(SettingsControl::Unmute), position))
        return SettingsControl::Unmute;
    if (!m_gameState.isMuted()) {
        const sf::FloatRect track = sliderTrackBounds();
        const sf::Vector2f knob = sliderKnobPosition();
        const sf::FloatRect knobBounds(
            { knob.x - SettingsScreenConfig::SliderKnobRadius,
              knob.y - SettingsScreenConfig::SliderKnobRadius },
            { SettingsScreenConfig::SliderKnobRadius * 2.f,
              SettingsScreenConfig::SliderKnobRadius * 2.f });
        if (GeometryUtils::contains(track, position) || GeometryUtils::contains(knobBounds, position))
            return SettingsControl::Slider;
    }
    if (GeometryUtils::contains(buttonBounds(0), position))
        return SettingsControl::Attribution;
    if (GeometryUtils::contains(buttonBounds(1), position))
        return SettingsControl::Help;
    if (GeometryUtils::contains(buttonBounds(2), position))
        return SettingsControl::Reset;
    return SettingsControl::None;
}

sf::FloatRect SettingsScreen::backButtonBounds() const {
    return {
        { SettingsScreenConfig::Padding, SettingsScreenConfig::Padding },
        { SettingsScreenConfig::BackButtonSize, SettingsScreenConfig::BackButtonSize }
    };
}

sf::FloatRect SettingsScreen::audioButtonBounds(SettingsControl control) const {
    const float totalWidth = SettingsScreenConfig::AudioButtonWidth * 2.f +
        SettingsScreenConfig::ButtonGap;
    const float startX = WindowConfig::Width / 2.f - totalWidth / 2.f;
    const float x = control == SettingsControl::Unmute
        ? startX + SettingsScreenConfig::AudioButtonWidth + SettingsScreenConfig::ButtonGap
        : startX;
    return {
        { x, SettingsScreenConfig::AudioButtonY },
        { SettingsScreenConfig::AudioButtonWidth, SettingsScreenConfig::AudioButtonHeight }
    };
}

sf::FloatRect SettingsScreen::sliderTrackBounds() const {
    return {
        { SettingsScreenConfig::SliderX, SettingsScreenConfig::SliderY - SettingsScreenConfig::SliderHeight / 2.f },
        { SettingsScreenConfig::SliderWidth, SettingsScreenConfig::SliderHeight }
    };
}

sf::FloatRect SettingsScreen::buttonBounds(int index) const {
    if (index == 2) {
        return {
            { WindowConfig::Width / 2.f - SettingsScreenConfig::ButtonWidth / 2.f,
              SettingsScreenConfig::ResetButtonY },
            { SettingsScreenConfig::ButtonWidth, SettingsScreenConfig::ButtonHeight }
        };
    }

    const float totalWidth = SettingsScreenConfig::PairedButtonWidth * 2.f +
        SettingsScreenConfig::ButtonGap;
    const float x = WindowConfig::Width / 2.f - totalWidth / 2.f +
        static_cast<float>(index) * (SettingsScreenConfig::PairedButtonWidth + SettingsScreenConfig::ButtonGap);
    return {
        { x, SettingsScreenConfig::PairedButtonsY },
        { SettingsScreenConfig::PairedButtonWidth, SettingsScreenConfig::ButtonHeight }
    };
}

sf::Vector2f SettingsScreen::sliderKnobPosition() const {
    const sf::FloatRect track = sliderTrackBounds();
    return {
        track.position.x +
            track.size.x * std::clamp(m_gameState.getMaxVolume(), 0.f, SettingsScreenConfig::MaxVolume) /
                SettingsScreenConfig::MaxVolume,
        SettingsScreenConfig::SliderY
    };
}

void SettingsScreen::draw(sf::RenderWindow& target) {
    target.draw(m_background);
    drawPanel(target);
    drawBackButton(target);
    drawTitle(target);
    drawAudioControls(target);
    drawSlider(target);
    drawActionButton(target, "Attribution", SettingsControl::Attribution, buttonBounds(0));
    drawActionButton(target, "Help", SettingsControl::Help, buttonBounds(1));
    drawActionButton(target, "Reset User Data", SettingsControl::Reset, buttonBounds(2), true);
    drawOverlay(target);
    if (m_attributionOverlay)
        target.draw(*m_attributionOverlay);
}

void SettingsScreen::drawBackButton(sf::RenderTarget& target) const {
    sf::RectangleShape button(backButtonBounds().size);
    button.setPosition(backButtonBounds().position);
    styleRectangle(
        button,
        m_hoveredControl == SettingsControl::Back ? SettingsScreenConfig::ButtonHoverColor : SettingsScreenConfig::ButtonColor,
        SettingsScreenConfig::PanelOutlineColor,
        SettingsScreenConfig::ButtonOutlineThickness);
    target.draw(button);
    target.draw(m_backSprite);
}

void SettingsScreen::drawTitle(sf::RenderTarget& target) const {
    target.draw(makeCenteredText(
        m_font,
        "Settings",
        SettingsScreenConfig::TitleTextSize,
        { WindowConfig::Width / 2.f, SettingsScreenConfig::TitleY },
        SettingsScreenConfig::TextColor));
}

void SettingsScreen::drawPanel(sf::RenderTarget& target) const {
    target.draw(m_panel);
}

void SettingsScreen::drawAudioControls(sf::RenderTarget& target) const {
    drawAudioButton(target, "Mute", GameObjects::Mute, SettingsControl::Mute, m_gameState.isMuted());
    drawAudioButton(target, "Sound On", GameObjects::VolumeHigh, SettingsControl::Unmute, !m_gameState.isMuted());
}

void SettingsScreen::drawAudioButton(sf::RenderTarget& target,
                                     const std::string& label,
                                     GameObjects icon,
                                     SettingsControl control,
                                     bool active) const {
    const sf::FloatRect bounds = audioButtonBounds(control);
    const bool hovered = m_hoveredControl == control;
    const sf::Color fill = active
        ? SettingsScreenConfig::ButtonHoverColor
        : (hovered ? SettingsScreenConfig::InactiveAudioButtonHoverColor : SettingsScreenConfig::ButtonColor);

    TextButton button(m_font, label, bounds);
    TextButtonMetrics metrics = ButtonStyles::settingsMetrics(SettingsScreenConfig::LabelTextSize, 16);
    metrics.textOffset = { SettingsScreenConfig::AudioLabelOffsetX, 0.f };
    button.setMetrics(metrics);
    button.setColors(ButtonStyles::settingsColors(fill, SettingsScreenConfig::InactiveAudioButtonHoverColor));
    button.setHovered(hovered && !active);
    button.setSelected(active);
    target.draw(button);

    sf::Sprite iconSprite(ResourceManager::instance().getImage(icon));
    fitSpriteTo(iconSprite, {
        { bounds.position.x + SettingsScreenConfig::AudioIconOffsetX,
          bounds.position.y + SettingsScreenConfig::AudioIconOffsetY },
        { SettingsScreenConfig::AudioIconSize, SettingsScreenConfig::AudioIconSize }
    });
    target.draw(iconSprite);

}

void SettingsScreen::drawSlider(sf::RenderTarget& target) const {
    const bool muted = m_gameState.isMuted();
    sf::Sprite low = m_volumeLowSprite;
    sf::Sprite high = m_volumeHighSprite;
    if (muted) {
        low.setColor(SettingsScreenConfig::DisabledColor);
        high.setColor(SettingsScreenConfig::DisabledColor);
    }
    target.draw(low);
    target.draw(high);

    const sf::FloatRect trackBounds = sliderTrackBounds();
    sf::RectangleShape track(trackBounds.size);
    track.setPosition(trackBounds.position);
    track.setFillColor(muted ? SettingsScreenConfig::DisabledColor : SettingsScreenConfig::SliderTrackColor);
    target.draw(track);

    sf::RectangleShape fill({
        trackBounds.size.x *
            std::clamp(m_gameState.getMaxVolume(), 0.f, SettingsScreenConfig::MaxVolume) /
            SettingsScreenConfig::MaxVolume,
        trackBounds.size.y
    });
    fill.setPosition(trackBounds.position);
    fill.setFillColor(muted ? SettingsScreenConfig::DisabledColor : SettingsScreenConfig::SliderFillColor);
    target.draw(fill);

    sf::CircleShape knob(SettingsScreenConfig::SliderKnobRadius);
    knob.setOrigin({ SettingsScreenConfig::SliderKnobRadius, SettingsScreenConfig::SliderKnobRadius });
    knob.setPosition(sliderKnobPosition());
    knob.setFillColor(muted ? SettingsScreenConfig::DisabledColor : SettingsScreenConfig::TextColor);
    knob.setOutlineColor(SettingsScreenConfig::PanelOutlineColor);
    knob.setOutlineThickness(SettingsScreenConfig::ButtonOutlineThickness);
    target.draw(knob);

    target.draw(makeCenteredText(
        m_font,
        std::to_string(static_cast<int>(std::round(m_gameState.getMaxVolume()))) + "%",
        SettingsScreenConfig::VolumeTextSize,
        { SettingsScreenConfig::SliderX + SettingsScreenConfig::SliderWidth / 2.f,
          SettingsScreenConfig::SliderY + SettingsScreenConfig::VolumeValueOffsetY },
        muted ? SettingsScreenConfig::MutedTextColor : SettingsScreenConfig::TextColor));
}

void SettingsScreen::drawActionButton(sf::RenderTarget& target,
                                      const std::string& label,
                                      SettingsControl control,
                                      sf::FloatRect bounds,
                                      bool danger) const {
    const bool hovered = m_hoveredControl == control;
    const sf::Color base = danger ? SettingsScreenConfig::DangerColor : SettingsScreenConfig::ButtonColor;
    const sf::Color hover = danger ? SettingsScreenConfig::DangerHoverColor : SettingsScreenConfig::ButtonHoverColor;

    TextButton button(m_font, label, bounds);
    button.setColors(ButtonStyles::settingsColors(base, hover));
    button.setMetrics(ButtonStyles::settingsMetrics(SettingsScreenConfig::ButtonTextSize, 18));
    button.setAvailableBounds({ { SettingsScreenConfig::PanelX, bounds.position.y },
        { SettingsScreenConfig::PanelWidth, bounds.size.y } });
    button.setHovered(hovered);
    target.draw(button);
}
