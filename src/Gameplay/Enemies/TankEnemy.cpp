#include "Gameplay/Enemies/TankEnemy.h"
#include "Gameplay/Constants.h"

TankEnemy::TankEnemy(const sf::Texture& texture, sf::Vector2f position, GameState& gameState,
    const Drone& drone, const CommandVehicle& vehicle)
    : Enemy(texture, position, TankEnemyConfig::FrameCount,
        TankEnemyConfig::FrameDuration, TankEnemyConfig::Scale,
        { TankEnemyConfig::MuzzleX, TankEnemyConfig::MuzzleY }, gameState, drone, vehicle)
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

// The 5 frames run across the telegraph window via dt; firing resets them for the next cycle.
void TankEnemy::updateAnimation(float dt) {
    if (!isFiring()) {        // not firing -> reset to the loaded frame 0
        m_animation.reset();
        return;
    }
    m_animation.update(dt);
    if (isFireRequested())    // the shot leaves right after the last frame -> restart the cycle
        m_animation.reset();
}