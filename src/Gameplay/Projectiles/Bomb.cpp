#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"

Bomb::Bomb(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : MovingObject(texture, position, { 0.f, BombConfig::FallSpeed }, gameState)
    , m_animation(Animation::frameSize(texture, BombConfig::FrameCount),
        BombConfig::FrameCount, BombConfig::FrameDuration)
{
    setupSprite(m_animation.getFrameRect(), BombConfig::Scale);
    gameState.addActiveBomb();
}

Bomb::~Bomb() {
    gameState().removeActiveBomb();
}

void Bomb::update(float dt)
{
    move(dt);
    m_animation.update(dt);
    m_sprite.setTextureRect(m_animation.getFrameRect());
    if (getPosition().y > WindowConfig::Height)
        destroy();
}