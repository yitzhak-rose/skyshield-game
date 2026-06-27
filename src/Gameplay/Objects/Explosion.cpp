#include "Gameplay/Objects/Explosion.h"

Explosion::Explosion(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : GameObject(texture, position, gameState)
    , m_animation(Animation::frameSize(texture, ExplosionConfig::FrameCount),
        ExplosionConfig::FrameCount, ExplosionConfig::FrameDuration)
{
    setupSprite(m_animation.getFrameRect(), ExplosionConfig::Scale);
}

void Explosion::update(float dt) {
    m_timer += dt;
    int frame = static_cast<int>(m_timer / ExplosionConfig::FrameDuration);

    // After the last frame, the effect is over — remove it from the world.
    if (frame >= ExplosionConfig::FrameCount) {
        destroy();
        return;
    }

    m_sprite.setTextureRect(m_animation.frameRect(frame));
}