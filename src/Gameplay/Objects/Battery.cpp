#include "Gameplay/Objects/Battery.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"

#include <algorithm>
#include <cmath>

Battery::Battery(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_animation(
        Animation::frameSize(texture, BatteryConfig::FrameCount),
        BatteryConfig::FrameCount,
        BatteryConfig::FrameDuration,
        false) {
    setupSprite(m_animation.getFrameRect(), BatteryConfig::Scale);
    updateFrame();
}

void Battery::update(float) {
    updateFrame();
}

void Battery::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite sprite = m_sprite;
    const sf::Vector2f position = sprite.getPosition();
    sprite.setPosition({ std::round(position.x), std::round(position.y) });
    sprite.setTextureRect(m_animation.frameRect(getFrameIndex()));
    target.draw(sprite, states);
}

void Battery::setDisplayHeight(float height) {
    const sf::IntRect frame = m_animation.getFrameRect();
    const float scale = height / static_cast<float>(frame.size.y);
    m_sprite.setScale({ scale, scale });
}

float Battery::getBatteryPercent() const {
    return gameState().getBatteryPercent();
}

void Battery::updateFrame() {
    m_sprite.setTextureRect(m_animation.frameRect(getFrameIndex()));
}

int Battery::getFrameIndex() const {
    const float batteryPercent = std::clamp(getBatteryPercent(), 0.f, BatteryConfig::FullPercent);
    const float emptyPercent = BatteryConfig::FullPercent - batteryPercent;
    const float frameStep = BatteryConfig::FullPercent / (BatteryConfig::FrameCount - 1);
    return std::clamp(static_cast<int>(std::round(emptyPercent / frameStep)),
        0, BatteryConfig::FrameCount - 1);
}
