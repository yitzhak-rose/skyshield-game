#include "Gameplay/Enemies/FastEnemy.h"
#include "Gameplay/Constants.h"

FastEnemy::FastEnemy(const sf::Texture& texture, sf::Vector2f position)
    : Enemy(texture, position, FastEnemyConfig::FrameCount,
        FastEnemyConfig::FrameDuration, FastEnemyConfig::Scale,
        { FastEnemyConfig::MuzzleX, FastEnemyConfig::MuzzleY })
{
}

void FastEnemy::onHit() {
    destroy();
}

int FastEnemy::getReward() const {
    return FastEnemyConfig::Reward;
}

float FastEnemy::getSpeed() const {
    return FastEnemyConfig::Speed;
}