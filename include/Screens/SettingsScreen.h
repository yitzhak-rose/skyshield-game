#pragma once

#include "App/State/GameState.h"
#include "Enums/GameObjects.h"
#include "Screens/Components/ScrollableTextOverlay.h"
#include "Screens/Core/Screen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class SettingsScreen : public Screen {
public:
    explicit SettingsScreen(GameState& gameState);

    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& target) override;

private:
    enum class Control {
        None,
        Back,
        Mute,
        Unmute,
        Slider,
        Help,
        Attribution,
        Reset
    };

    void configureLayout();
    void handleClick(sf::Vector2f position);
    void handleDrag(sf::Vector2f position);
    void updateHover(sf::Vector2f position);
    void updateVolumeFromPosition(float x);
    void syncAudioSettings() const;
    void showResetConfirmation();
    void showAttribution();

    Control controlAt(sf::Vector2f position) const;
    sf::FloatRect backButtonBounds() const;
    sf::FloatRect audioButtonBounds(Control control) const;
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
                         Control control,
                         bool active) const;
    void drawSlider(sf::RenderTarget& target) const;
    void drawActionButton(sf::RenderTarget& target,
                          const std::string& label,
                          Control control,
                          sf::FloatRect bounds,
                          bool danger = false) const;

    GameState& m_gameState;
    sf::RectangleShape m_background;
    sf::RectangleShape m_panel;
    sf::Sprite m_backSprite;
    sf::Sprite m_volumeLowSprite;
    sf::Sprite m_volumeHighSprite;
    const sf::Font& m_font;
    Control m_hoveredControl = Control::None;
    bool m_draggingSlider = false;
    std::unique_ptr<ScrollableTextOverlay> m_attributionOverlay;
};
