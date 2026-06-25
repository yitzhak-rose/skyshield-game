#include "Gameplay/Objects/CommandVehicle.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"
#include <cmath>

// Builds the vehicle, then crops the sprite to one frame and centers its origin.
CommandVehicle::CommandVehicle(
    const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_animation(Animation::frameSize(texture, VehicleConfig::FrameCount),
        VehicleConfig::FrameCount, VehicleConfig::FrameDuration)
    , m_basePosition(position)
{
    setupSprite(m_animation.getFrameRect(), VehicleConfig::Scale);
}

// Loops frames 0..3 (flashing beacon) while healthy, or 4..5 (burning) when critical, and runs the hit-shake.
void CommandVehicle::update(float dt) {
    updateAnimationFrame(dt);
    updateShake(dt);
}

void CommandVehicle::updateAnimationFrame(float dt) {
    m_frameTimer += dt;
    int step = static_cast<int>(m_frameTimer / VehicleConfig::FrameDuration);

    const int frame = (getArmorPercent() >= VehicleConfig::CriticalArmor)
        ? step % VehicleConfig::NormalFrames
        : VehicleConfig::NormalFrames + step % (VehicleConfig::FrameCount - VehicleConfig::NormalFrames);

    m_sprite.setTextureRect(m_animation.frameRect(frame));
}

// Briefly shakes after a hit, decaying back to the resting position.
void CommandVehicle::updateShake(float dt) {
    if (m_shakeTimer <= 0.f) {
        m_sprite.setPosition(m_basePosition);
        return;
    }

    m_shakeTimer -= dt;
    float decay = m_shakeTimer / VehicleConfig::ShakeDuration; // 1 -> 0
    float offsetX = std::sin(m_shakeTimer * VehicleConfig::ShakeSpeed) * VehicleConfig::ShakeMagnitude * decay;
    m_sprite.setPosition({ m_basePosition.x + offsetX, m_basePosition.y });
}

float CommandVehicle::getArmorPercent() const {
    return gameState().getVehicleArmorPercent();
}

void CommandVehicle::takeDamage() {
    gameState().damageVehicle(VehicleConfig::DamagePerHit);
    m_shakeTimer = VehicleConfig::ShakeDuration; // kick off a fresh shake on every hit
    if (getArmorPercent() <= 0.f)
        destroy();
}