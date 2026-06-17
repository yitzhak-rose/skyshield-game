#include "Gameplay/Objects/Drone.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"

// Builds the drone, then crops the sprite to one frame and centers its origin.
Drone::Drone(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : MovingObject(texture, position, { 0.f, 0.f }, gameState)
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

    gameState().updateBattery(dt);
}

bool Drone::canDropBomb() const {
    return gameState().getBombsAvailable() > 0;
}

bool Drone::consumeAmmo() {
    return gameState().consumeBomb();
}

float Drone::getBatteryPercent() const {
    return gameState().getBatteryPercent();
}

int Drone::getRemainingAmmo() const {
    return gameState().getBombsAvailable();
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
