#include "CommandVehicle.h"
#include "Constants.h"

CommandVehicle::CommandVehicle(const sf::Texture &texture, sf::Vector2f position)
    : GameObject(texture, position)
{
}

void CommandVehicle::update(float /*dt*/) {
    // The vehicle is static — nothing to update.
}

void CommandVehicle::takeDamage() {
    m_armor -= VehicleConfig::DamagePerHit;
    if (m_armor <= 0.f) {
        m_armor = 0.f;
        destroy();
    }
}