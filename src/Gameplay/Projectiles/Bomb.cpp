#include "Gameplay/Projectiles/Bomb.h"
#include "App/AssetConstants.h"
#include "App/State/GameState.h"

Bomb::Bomb(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : MovingObject(texture, position, { 0.f, BombConfig::FallSpeed }, gameState)
    , m_animation(Animation::frameSize(texture, BombConfig::Sprite.frameCount),
        BombConfig::Sprite.frameCount, BombConfig::Sprite.frameDuration)
{
    setupSprite(m_animation.getFrameRect(), BombConfig::Sprite.scale);
    m_animation.applyInitialFrame(m_sprite);
    gameState.addActiveBomb();
}

Bomb::~Bomb() {
    gameState().removeActiveBomb();
}

void Bomb::update(float dt) {
    m_animation.apply(m_sprite, dt);
    move(dt);
    if (getPosition().y > WindowConfig::Height + getBounds().size.y)
        destroy();
}
