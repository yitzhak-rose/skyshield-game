#include "Gameplay/Objects/Drone.h"
#include "Gameplay/Constants.h"

// Builds the drone, then crops the sprite to one frame and centers its origin.
Drone::Drone(const sf::Texture& texture, sf::Vector2f position)
    : MovingObject(texture, position, { 0.f, 0.f })
    , m_animation(Animation::frameSize(texture, DroneConfig::FrameCount), DroneConfig::FrameCount, DroneConfig::FrameDuration)
{
    setupSprite(m_animation.getFrameRect(), DroneConfig::Scale);
}

// Moves, advances the rotor animation, then drains the battery.
void Drone::update(float dt)
{
    move(dt);
    m_animation.update(dt);
    m_sprite.setTextureRect(m_animation.getFrameRect());

    m_battery -= m_drainRate * dt; // drain rate × time; rate set per stage/upgrade
    if (m_battery < 0.f)
        m_battery = 0.f;
}

void Drone::moveLeft()
{
    m_velocity.x = -DroneConfig::Speed;
    m_sprite.setScale({ -DroneConfig::Scale, DroneConfig::Scale }); // mirror to face left
}

void Drone::moveRight()
{
    m_velocity.x = DroneConfig::Speed;
    m_sprite.setScale({ DroneConfig::Scale, DroneConfig::Scale }); // normal, facing right
}

void Drone::stop()
{
    m_velocity.x = 0.f;
}
