#include "Gameplay/Objects/Battery.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"
#include <cmath>

// Builds the battery and crops the sprite to its first frame.
Battery::Battery(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_animation(Animation::frameSize(texture, BatteryConfig::FrameCount),
        BatteryConfig::FrameCount, BatteryConfig::FrameDuration, false)
{
    setupSprite(m_animation.getFrameRect(), BatteryConfig::Scale);
}

// Draws the charge-appropriate frame to the menu bar, rounded to whole pixels.
void Battery::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite sprite = m_sprite;
    const sf::Vector2f position = sprite.getPosition();
    sprite.setPosition({ std::round(position.x), std::round(position.y) });
    sprite.setTextureRect(m_animation.frameRect(getFrameIndex()));
    target.draw(sprite, states);
}

// Returns the current charge percentage.
float Battery::getBatteryPercent() const {
    return gameState().getBatteryPercent();
}

// Maps the current charge percentage to a frame index (full -> 0, empty -> last).
int Battery::getFrameIndex() const {
    const float emptyPercent = BatteryConfig::FullPercent - getBatteryPercent();
    const float frameStep = BatteryConfig::FullPercent / (BatteryConfig::FrameCount - 1);
    return static_cast<int>(std::round(emptyPercent / frameStep));
}