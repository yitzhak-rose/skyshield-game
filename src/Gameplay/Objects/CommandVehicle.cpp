#include "Gameplay/Objects/CommandVehicle.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"

// Builds the vehicle, then crops the sprite to one frame and centers its origin.
CommandVehicle::CommandVehicle(
    const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_animation(Animation::frameSize(texture, VehicleConfig::FrameCount),
        VehicleConfig::FrameCount, VehicleConfig::FrameDuration)
{
    setupSprite(m_animation.getFrameRect(), VehicleConfig::Scale);
}

// Loops frames 0..3 (flashing beacon) while healthy, or 4..5 (burning) when critical.
void CommandVehicle::update(float dt) {
    m_frameTimer += dt;
    int step = static_cast<int>(m_frameTimer / VehicleConfig::FrameDuration);

    int frame;
    if (getArmorPercent() >= VehicleConfig::CriticalArmor)
        frame = step % VehicleConfig::NormalFrames;
    else {
        int burningCount = VehicleConfig::FrameCount - VehicleConfig::NormalFrames;
        frame = VehicleConfig::NormalFrames + step % burningCount;
    }
    m_sprite.setTextureRect(m_animation.frameRect(frame));
}

void CommandVehicle::takeDamage() {
    gameState().damageVehicle(VehicleConfig::DamagePerHit);
    if (getArmorPercent() <= 0.f)
        destroy();
}

float CommandVehicle::getArmorPercent() const {
    return gameState().getVehicleArmorPercent();
}
