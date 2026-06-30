#pragma once

#include "App/State/GameState.h"
#include "Enums/GameObjects.h"
#include "Enums/SettingsAction.h"
#include "Enums/SettingsControl.h"
#include "Screens/Components/ScrollableTextOverlay.h"
#include "Screens/Core/Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class SettingsScreen : public Screen {
public:
    explicit SettingsScreen(const GameState& gameState);

    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& target) override;
    SettingsAction consumeAction();
    bool consumeVolumeChange(float& volume);

private:
    void configureLayout();
    void handleClick(sf::Vector2f position);
    void handleDrag(sf::Vector2f position);
    void updateHover(sf::Vector2f position);
    void updateVolumeFromPosition(float x);
    void showResetConfirmation();
    void showAttribution();
    void requestAction(SettingsAction action);

    SettingsControl controlAt(sf::Vector2f position) const;
    sf::FloatRect backButtonBounds() const;
    sf::FloatRect audioButtonBounds(SettingsControl control) const;
    sf::FloatRect sliderTrackBounds() const;
    sf::FloatRect buttonBounds(int index) const;
    sf::Vector2f sliderKnobPosition() const;

    void drawBackButton(sf::RenderTarget& target) const;
    void drawTitle(sf::RenderTarget& target) const;
    void drawPanel(sf::RenderTarget& target) const;
    void drawAudioControls(sf::RenderTarget& target) const;
    void drawAudioButton(sf::RenderTarget& target,
                         const std::string& label,
                         GameObjects icon,
                         SettingsControl control,
                         bool active) const;
    void drawSlider(sf::RenderTarget& target) const;
    void drawActionButton(sf::RenderTarget& target,
                          const std::string& label,
                          SettingsControl control,
                          sf::FloatRect bounds,
                          bool danger = false) const;

    const GameState& m_gameState;
    sf::RectangleShape m_background;
    sf::RectangleShape m_panel;
    sf::Sprite m_backSprite;
    sf::Sprite m_volumeLowSprite;
    sf::Sprite m_volumeHighSprite;
    const sf::Font& m_font;
    SettingsControl m_hoveredControl = SettingsControl::None;
    bool m_draggingSlider = false;
    SettingsAction m_requestedAction = SettingsAction::None;
    bool m_hasVolumeChange = false;
    float m_requestedVolume = 100.f;
    std::unique_ptr<ScrollableTextOverlay> m_attributionOverlay;
};
