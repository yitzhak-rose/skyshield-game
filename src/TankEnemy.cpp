#include "TankEnemy.h"
#include "Constants.h"

TankEnemy::TankEnemy(const sf::Texture& texture, sf::Vector2f position)
    : Enemy(texture, position, TankEnemyConfig::FrameCount,
        TankEnemyConfig::FrameDuration, TankEnemyConfig::Scale)
{
}

void TankEnemy::onHit() {
    if (!m_wounded) {
        m_wounded = true;
        m_sprite.setColor(sf::Color(150, 150, 150)); // darken to show damage
    }
    else {
        destroy();
    }
}

int TankEnemy::getReward() const {
    return TankEnemyConfig::Reward;
}

float TankEnemy::getSpeed() const {
    return TankEnemyConfig::Speed;
}