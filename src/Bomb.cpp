#include "Bomb.h"
#include "Constants.h"

Bomb::Bomb(const sf::Texture &texture, sf::Vector2f position)
    : MovingObject(texture, position, {0.f, BombConfig::FallSpeed})
{
}

void Bomb::update(float dt)
{
    move(dt);
    if (getPosition().y > WindowConfig::Height)
        destroy();
}

