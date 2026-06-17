#include "Gameplay/Objects/Battery.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"

#include <cmath>

Battery::Battery(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_animation(
        Animation::frameSize(texture, BatteryConfig::FrameCount),
        BatteryConfig::FrameCount,
        BatteryConfig::FrameDuration,
        false) {
    setupSprite(m_animation.getFrameRect(), BatteryConfig::Scale);
}

void Battery::update(float) {
    m_sprite.setTextureRect(m_animation.frameRect(getFrameIndex()));
}

void Battery::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite sprite = m_sprite;
    const sf::Vector2f position = sprite.getPosition();
    sprite.setPosition({ std::round(position.x), std::round(position.y) });
    sprite.setTextureRect(m_animation.frameRect(getFrameIndex()));
    target.draw(sprite, states);
}

float Battery::getBatteryPercent() const {
    return gameState().getBatteryPercent();
}

int Battery::getFrameIndex() const {
    const float emptyPercent = BatteryConfig::FullPercent - getBatteryPercent();
    const float frameStep = BatteryConfig::FullPercent / (BatteryConfig::FrameCount - 1);
    return static_cast<int>(std::round(emptyPercent / frameStep));
}
