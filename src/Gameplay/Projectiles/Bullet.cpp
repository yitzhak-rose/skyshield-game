#include "Gameplay/Projectiles/Bullet.h"
#include "Gameplay/Constants.h"

// Centers the single-frame sprite and faces it along its travel direction.
Bullet::Bullet(const sf::Texture& texture, sf::Vector2f position, float velocityX)
    : MovingObject(texture, position, { velocityX, 0.f })
{
}

void Bullet::update(float dt)
{
    move(dt);
    if (getPosition().x < 0.f || getPosition().x > WindowConfig::Width)
        destroy();
}
