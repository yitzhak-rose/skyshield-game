#include "Gameplay/Projectiles/Bullet.h"
#include "Gameplay/Constants.h"

// Faces the flash along its travel direction; the sheet's flame points right by default.
Bullet::Bullet(const sf::Texture& texture, sf::Vector2f position,
    float velocityX, GameState& gameState)
    : MovingObject(texture, position, { velocityX, 0.f }, gameState)
{
    if (velocityX < 0.f)
        m_sprite.setScale({ -1.f, 1.f }); // mirror around the muzzle so the flame points left
}

void Bullet::update(float dt)
{
    move(dt);
    if (getPosition().x < 0.f || getPosition().x > WindowConfig::Width)
        destroy();
}
