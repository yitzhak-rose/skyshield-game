#include "MovingObject.h"

MovingObject::MovingObject(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f velocity)
    : GameObject(texture, position)
    , m_velocity(velocity)
{
}

void MovingObject::move(float dt) {
    setPosition(getPosition() + m_velocity * dt);
}