#include "Gameplay/Enemies/Enemy.h"
#include "Gameplay/Factory.h"
#include "App/AssetConstants.h"
#include "App/State/GameState.h"
#include "Utilities/Utils.h"
#include <algorithm>
#include <cmath>

Enemy::Enemy(const sf::Texture& texture, sf::Vector2f position,
    const EnemyTypeConfig& stats, GameState& gameState,
    const GameObject& player, const GameObject& target)
    : MovingObject(texture, position, { 0.f, 0.f }, gameState)
    , m_animation(Animation::frameSize(texture, stats.sprite.frameCount),
        stats.sprite.frameCount, stats.sprite.frameDuration)
    , m_player(player)
    , m_target(target)
    , m_stopRange(Utils::randomFloat(EnemyConfig::StopRangeMin, EnemyConfig::StopRangeMax))
    , m_muzzleOffset({ stats.muzzle.x, stats.muzzle.y })
{
    setupSprite(m_animation.getFrameRect(), stats.sprite.scale);
}

// MAIN LOOP & AI (The Brain)
#pragma region MAIN_LOOP
void Enemy::update(float dt) {
    decideState();
    switch (m_state) {
    case EnemyState::Advancing: advance(dt);      break;
    case EnemyState::Panicking: panic(dt);        break;
    case EnemyState::Firing:    updateFiring(dt); break;
    }
    updateFacing();
    updateAnimation(dt);
    m_sprite.setTextureRect(m_animation.getFrameRect());
}

void Enemy::decideState() {
    m_state = isDroneInRange() ? EnemyState::Panicking
        : isInFiringRange() ? EnemyState::Firing
        : EnemyState::Advancing;
}

void Enemy::advance(float dt) {
    m_velocity.x = directionToVehicle() * getSpeed();
    move(dt);
}

void Enemy::panic(float dt) {
    float x = getPosition().x;
    auto [leftWall, rightWall] = calculateWalls(x);
    updateUTurnState(x - leftWall, rightWall - x);

    if (!m_isUTurning)
        m_fleeDirection = (playerX() < x) ? 1.f : -1.f;

    m_velocity.x = m_fleeDirection * getSpeed() * EnemyConfig::PanicMultiplier;
    move(dt);
    setPosition({ std::clamp(getPosition().x, leftWall, rightWall), getPosition().y });
}

void Enemy::updateFiring(float dt) {
    m_velocity.x = 0.f;
    m_fireTimer += dt;
    if (m_fireTimer >= EnemyConfig::TelegraphTime) {
        m_fireRequested = true;
        m_fireTimer = 0.f;
    }
}
#pragma endregion

// GRAPHICS & ANIMATION
#pragma region GRAPHICS
// Mirrors the sprite when moving left; the art faces right by default.
void Enemy::updateFacing() {
    float flip;
    if (m_velocity.x != 0.f)        flip = (m_velocity.x < 0.f) ? -1.f : 1.f;  // moving: face travel direction
    else if (isFiring())            flip = directionToVehicle();               // stopped to fire: face vehicle
    else                            return;                                    // otherwise keep current facing

    float scale = m_sprite.getScale().y; // y never flips, so it holds the clean magnitude
    m_sprite.setScale({ flip * scale, scale });
}

// Default: animate continuously (used by the fast enemy's driving loop).
void Enemy::updateAnimation(float dt) {
    m_animation.update(dt);
}
#pragma endregion

// SENSORS & HELPERS (Live Reads)
#pragma region SENSORS
// Live reads of the drone and vehicle, so the AI always sees their current state.
float Enemy::targetX() const { return m_target.getPosition().x; }
float Enemy::targetHalfWidth() const { return m_target.getBounds().size.x / 2.f; }
float Enemy::playerX() const { return m_player.getPosition().x; }

// Returns the sign of the direction from the enemy toward the vehicle.
float Enemy::directionToVehicle() const {
    return (targetX() < getPosition().x) ? -1.f : 1.f;
}

// Hysteretic panic trigger: enter at PanicRange, leave only past PanicExitRange.
bool Enemy::isDroneInRange() const {
    float range = (m_state == EnemyState::Panicking) ? EnemyConfig::PanicExitRange : EnemyConfig::PanicRange;
    return std::abs(playerX() - getPosition().x) < range;
}

bool Enemy::isInFiringRange() const {
    float centerDist = std::abs(targetX() - getPosition().x);
    float gap = centerDist - targetHalfWidth() - getBounds().size.x / 2.f;
    return gap <= m_stopRange;
}
#pragma endregion

// PHYSICS & SPATIAL LOGIC
#pragma region PHYSICS

// Computes the left/right movement bounds for the enemy's center,
// inset by its own half-width so the whole body stays inside the bounds.
std::pair<float, float> Enemy::calculateWalls(float x) const {
    float half = getBounds().size.x / 2.f;
    float gap = targetHalfWidth() + EnemyConfig::VehicleGap;
    bool leftOfVehicle = (x < targetX());
    float leftWall = leftOfVehicle ? half : targetX() + gap + half;
    float rightWall = leftOfVehicle ? targetX() - gap - half : WindowConfig::Width - half;
    return { leftWall, rightWall };
}

// Updates whether the enemy is mid-U-turn (starts one at a wall, ends it after UTurnDistance).
void Enemy::updateUTurnState(float distToLeft, float distToRight) {
    if (m_isUTurning) {
        if ((m_fleeDirection == 1.f && distToLeft > EnemyConfig::UTurnDistance) ||
            (m_fleeDirection == -1.f && distToRight > EnemyConfig::UTurnDistance))
            m_isUTurning = false;
    }
    else {
        if (distToLeft <= EnemyConfig::UTurnTrigger) {
            m_isUTurning = true;
            m_fleeDirection = 1.f;
        }
        else if (distToRight <= EnemyConfig::UTurnTrigger) {
            m_isUTurning = true;
            m_fleeDirection = -1.f;
        }
    }
}
#pragma endregion

// WEAPON SYSTEM
#pragma region WEAPON
// When a shot is queued, the enemy computes the velocity and hands the World a bullet from the Factory.
std::unique_ptr<GameObject> Enemy::takeSpawn() {
    if (m_explosionRequested) {
        m_explosionRequested = false;
        return Factory::create(GameObjects::Explosion, getPosition(), gameState());
    }

    if (!m_fireRequested)
        return nullptr;

    m_fireRequested = false;
    return Factory::createBullet(getMuzzlePosition(), getBulletVelocityX(), gameState());
}
// Offsets the muzzle toward the vehicle, so bullets leave the gun, not the body center.
sf::Vector2f Enemy::getMuzzlePosition() const {
    return getPosition() + sf::Vector2f{ directionToVehicle() * m_muzzleOffset.x, m_muzzleOffset.y };
}

// Builds the bullet's horizontal velocity from the firing direction and the bullet speed.
float Enemy::getBulletVelocityX() const {
    return directionToVehicle() * BulletConfig::Speed;
}
#pragma endregion

void Enemy::defeat() {
    if (isDead())
        return;

    destroy();
    gameState().recordEnemyDefeated(getReward());
    m_explosionRequested = true;    

}
