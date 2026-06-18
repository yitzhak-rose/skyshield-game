#include "Gameplay/Objects/MovingObject.h"

// Forwards to GameObject and stores the starting velocity.
MovingObject::MovingObject(const sf::Texture& texture, sf::Vector2f position,
    sf::Vector2f velocity, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_velocity(velocity)
{
}

// Advances the position by velocity * dt.
void MovingObject::move(float dt) {
    setPosition(getPosition() + m_velocity * dt);
}