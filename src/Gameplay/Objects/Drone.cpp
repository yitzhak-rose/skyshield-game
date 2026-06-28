#include "Gameplay/Objects/Drone.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"
#include "Gameplay/Factory.h"
#include <algorithm>

// Builds the drone, then crops the sprite to one frame and centers its origin.
Drone::Drone(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : MovingObject(texture, position, { 0.f, 0.f }, gameState)
    , m_animation(Animation::frameSize(texture, DroneConfig::Sprite.frameCount),
        DroneConfig::Sprite.frameCount, DroneConfig::Sprite.frameDuration)
{
    const sf::IntRect frame = m_animation.getFrameRect();
    m_displayScale = std::min(
        DroneConfig::DisplayWidth / static_cast<float>(frame.size.x),
        DroneConfig::DisplayHeight / static_cast<float>(frame.size.y));
    setupSprite(frame, m_displayScale);
}

// Moves, advances the rotor animation, then drains the battery.
void Drone::update(float dt)
{
    handleInput();         
    handleBombDrop();      
    move(dt);
    // Keep the whole drone on screen; it only ever moves horizontally.
    float halfWidth = getBounds().size.x * 0.5f;
    sf::Vector2f pos = getPosition();
    pos.x = std::clamp(pos.x, halfWidth, WindowConfig::Width - halfWidth);
    m_sprite.setPosition(pos);

    m_animation.apply(m_sprite, dt);

    gameState().updateBattery(dt);
}

void Drone::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        moveLeft();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        moveRight();
    else
        stop();
}

void Drone::handleBombDrop() {
    const bool spaceNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (spaceNow && !m_spaceWasPressed && consumeAmmo())
        m_bombRequested = true;
    m_spaceWasPressed = spaceNow;
}

std::unique_ptr<GameObject> Drone::takeSpawn() {
    if (!m_bombRequested)
        return nullptr;
    m_bombRequested = false;
    return Factory::create(GameObjects::Bomb, getPosition(), gameState());
}

bool Drone::consumeAmmo() {
    return gameState().consumeBomb();
}

void Drone::moveLeft()
{
    m_velocity.x = -DroneConfig::Speed;
    m_sprite.setScale({ -m_displayScale, m_displayScale }); // mirror to face left
}

void Drone::moveRight()
{
    m_velocity.x = DroneConfig::Speed;
    m_sprite.setScale({ m_displayScale, m_displayScale }); // normal, facing right
}

void Drone::stop()
{
    m_velocity.x = 0.f;
}
