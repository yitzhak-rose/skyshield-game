#include "CommandVehicle.h"
#include "Constants.h"

// Builds the vehicle, then crops the sprite to one frame and centers its origin.
CommandVehicle::CommandVehicle(const sf::Texture& texture, sf::Vector2f position)
    : GameObject(texture, position)
    , m_animation(Animation::frameSize(texture, VehicleConfig::FrameCount),
        VehicleConfig::FrameCount, VehicleConfig::FrameDuration)
{
    setupSprite(m_animation.getFrameRect(), VehicleConfig::Scale);
}

// Advances the smoke and beacon animation; the vehicle never moves.
void CommandVehicle::update(float dt) {
    m_animation.update(dt);
    m_sprite.setTextureRect(m_animation.getFrameRect());
}

void CommandVehicle::takeDamage() {
    m_armor -= VehicleConfig::DamagePerHit;
    if (m_armor <= 0.f) {
        m_armor = 0.f;
        destroy();
    }
}