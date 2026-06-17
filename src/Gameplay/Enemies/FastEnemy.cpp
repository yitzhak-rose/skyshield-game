#include "Gameplay/Enemies/FastEnemy.h"
#include "Gameplay/Constants.h"

FastEnemy::FastEnemy(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : Enemy(texture, position, FastEnemyConfig::FrameCount,
        FastEnemyConfig::FrameDuration, FastEnemyConfig::Scale, gameState)
{
}

void FastEnemy::onHit() {
    defeat();
}

int FastEnemy::getReward() const {
    return FastEnemyConfig::Reward;
}

float FastEnemy::getSpeed() const {
    return FastEnemyConfig::Speed;
}
