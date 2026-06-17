#include "Gameplay/Objects/MovingObject.h"

MovingObject::MovingObject(const sf::Texture& texture, sf::Vector2f position,
    sf::Vector2f velocity, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_velocity(velocity)
{
}

void MovingObject::move(float dt) {
    setPosition(getPosition() + m_velocity * dt);
}
