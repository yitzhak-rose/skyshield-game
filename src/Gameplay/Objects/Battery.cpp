#include "Gameplay/Objects/Battery.h"
#include "Gameplay/Constants.h"

#include <algorithm>
#include <cmath>

Battery::Battery(const sf::Texture& texture, sf::Vector2f position)
    : GameObject(texture, position)
    , m_animation(
        Animation::frameSize(texture, BatteryConfig::FrameCount),
        BatteryConfig::FrameCount,
        BatteryConfig::FrameDuration,
        false)
    , m_batteryPercent(BatteryConfig::FullPercent) {
    setupSprite(m_animation.getFrameRect(), BatteryConfig::Scale);
}

void Battery::update(float) {
}

void Battery::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite sprite = m_sprite;
    const sf::Vector2f position = sprite.getPosition();
    sprite.setPosition({ std::round(position.x), std::round(position.y) });
    target.draw(sprite, states);
}

void Battery::setBatteryPercent(float percent) {
    m_batteryPercent = std::clamp(percent, 0.f, BatteryConfig::FullPercent);
    m_sprite.setTextureRect(m_animation.frameRect(getFrameIndex()));
}

float Battery::getBatteryPercent() const {
    return m_batteryPercent;
}

int Battery::getFrameIndex() const {
    const float emptyPercent = BatteryConfig::FullPercent - m_batteryPercent;
    const float frameStep = BatteryConfig::FullPercent / (BatteryConfig::FrameCount - 1);
    return static_cast<int>(std::round(emptyPercent / frameStep));
}
