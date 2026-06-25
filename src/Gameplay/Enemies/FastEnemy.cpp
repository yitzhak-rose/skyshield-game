#include "Gameplay/Enemies/FastEnemy.h"
#include "Gameplay/Constants.h"

FastEnemy::FastEnemy(const sf::Texture& texture, sf::Vector2f position, GameState& gameState,
    const Drone& drone, const CommandVehicle& vehicle)
    : Enemy(texture, position, FastEnemyConfig::FrameCount,
        FastEnemyConfig::FrameDuration, FastEnemyConfig::Scale,
        { FastEnemyConfig::MuzzleX, FastEnemyConfig::MuzzleY }, gameState, drone, vehicle)
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