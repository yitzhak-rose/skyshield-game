#include "Bullet.h"
#include "Constants.h"

// Centers the single-frame sprite and faces it along its travel direction.
Bullet::Bullet(const sf::Texture& texture, sf::Vector2f position, float velocityX)
    : MovingObject(texture, position, { velocityX, 0.f })
{
   /* sf::IntRect frame({0, 0}, sf::Vector2i(texture.getSize()));
    setupSprite(frame, BulletConfig::Scale);
    if (velocityX > 0.f) // art's tip points left; flip when flying right
        m_sprite.setScale({ BulletConfig::Scale, BulletConfig::Scale });*/
}

void Bullet::update(float dt)
{
    move(dt);
    if (getPosition().x < 0.f || getPosition().x > WindowConfig::Width)
        destroy();
}