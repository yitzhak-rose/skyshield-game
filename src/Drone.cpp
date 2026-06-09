#include "Drone.h"
#include "Constants.h"

Drone::Drone(const sf::Texture &texture, sf::Vector2f position)
    : MovingObject(texture, position, {0.f, 0.f})
{
}

void Drone::update(float dt)
{
    move(dt);
    m_battery -= m_drainRate * dt; // drain rate × time; rate set per stage/upgrade
    if (m_battery < 0.f)
        m_battery = 0.f;
}

void Drone::moveLeft()
{
    m_velocity.x = -DroneConfig::Speed;
    m_sprite.setScale({-1.f, 1.f}); // mirror the sprite to face left
}

void Drone::moveRight()
{
    m_velocity.x = DroneConfig::Speed;
    m_sprite.setScale({1.f, 1.f}); // normal orientation, facing right
}

void Drone::stop()
{
    m_velocity.x = 0.f;
}