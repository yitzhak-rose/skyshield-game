#include "Gameplay/Enemies/FastEnemy.h"
#include "Gameplay/Constants.h"

FastEnemy::FastEnemy(const sf::Texture& texture, sf::Vector2f position, GameState& gameState,
    const GameObject& player, const GameObject& target)
    : Enemy(texture, position, FastEnemyConfig::Stats, gameState, player, target)
{
}

void FastEnemy::onHit() {
    defeat();
}

int FastEnemy::getReward() const {
    return FastEnemyConfig::Stats.reward;
}

float FastEnemy::getSpeed() const {
    return FastEnemyConfig::Stats.speed;
}
