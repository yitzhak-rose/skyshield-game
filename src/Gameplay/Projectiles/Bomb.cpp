#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/Constants.h"

// Builds the bomb, then crops the sprite to one frame and centers its origin.
Bomb::Bomb(const sf::Texture& texture, sf::Vector2f position)
    : MovingObject(texture, position, { 0.f, BombConfig::FallSpeed })
    , m_animation(Animation::frameSize(texture, BombConfig::FrameCount),
        BombConfig::FrameCount, BombConfig::FrameDuration)
{
    setupSprite(m_animation.getFrameRect(), BombConfig::Scale);
}

// Falls, wobbles, and dies once it leaves the bottom of the screen.
void Bomb::update(float dt)
{
    move(dt);
    m_animation.update(dt);
    m_sprite.setTextureRect(m_animation.getFrameRect());
    if (getPosition().y > WindowConfig::Height)
        destroy();
}