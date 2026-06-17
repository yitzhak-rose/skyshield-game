#include "Gameplay/Enemies/TankEnemy.h"
#include "Gameplay/Constants.h"

TankEnemy::TankEnemy(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : Enemy(texture, position, TankEnemyConfig::FrameCount,
        TankEnemyConfig::FrameDuration, TankEnemyConfig::Scale, gameState)
{
}

void TankEnemy::onHit() {
    if (!m_wounded) {
        m_wounded = true;
        m_sprite.setColor(sf::Color(150, 150, 150)); // darken to show damage
    }
    else {
        defeat();
    }
}

int TankEnemy::getReward() const {
    return TankEnemyConfig::Reward;
}

float TankEnemy::getSpeed() const {
    return TankEnemyConfig::Speed;
}

// Plays the muzzle-flash sequence only while firing; rewinds to frame 0 otherwise.
void TankEnemy::updateAnimation(float dt) {
    if (isFiring())
        m_animation.update(dt);
    else
        m_animation.reset();
}
