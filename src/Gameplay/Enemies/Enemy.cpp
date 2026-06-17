#include "Gameplay/Enemies/Enemy.h"
#include "Gameplay/Constants.h"
#include "Utilities/Utils.h"
#include <algorithm>
#include <cmath>

Enemy::Enemy(const sf::Texture& texture, sf::Vector2f position,
    int frameCount, float frameDuration, float scale, sf::Vector2f muzzleOffset)
    : MovingObject(texture, position, { 0.f, 0.f })
    , m_animation(Animation::frameSize(texture, frameCount), frameCount, frameDuration)
    , m_stopRange(randomFloat(EnemyConfig::StopRangeMin, EnemyConfig::StopRangeMax))
    , m_muzzleOffset(muzzleOffset)
{
    setupSprite(m_animation.getFrameRect(), scale);
}

// Offsets the muzzle toward the vehicle, so bullets leave the gun, not the body center.
sf::Vector2f Enemy::getMuzzlePosition() const {
    float facing = (m_vehicleX < getPosition().x) ? -1.f : 1.f;
    return getPosition() + sf::Vector2f{ facing * m_muzzleOffset.x, m_muzzleOffset.y };
}

void Enemy::update(float dt) {
    decideState();
    switch (m_state) {
    case EnemyState::Advancing:  advance(dt);       break;
    case EnemyState::Panicking:  panic(dt);         break;
    case EnemyState::Firing:     updateFiring(dt);  break;
    }
    updateFacing();
    updateAnimation(dt);
    m_sprite.setTextureRect(m_animation.getFrameRect());
}

// Default: animate continuously (used by the fast enemy's driving loop).
void Enemy::updateAnimation(float dt) {
    m_animation.update(dt);
}

// Mirrors the sprite when moving left; the art faces right by default.
void Enemy::updateFacing() {
    float scale = m_sprite.getScale().y; // y never flips, so it holds the clean magnitude

    float flip;
    if (m_velocity.x != 0.f)
        flip = (m_velocity.x < 0.f) ? -1.f : 1.f;      // moving: face the travel direction
    else if (isFiring())
        flip = (m_vehicleX < getPosition().x) ? -1.f : 1.f; // stopped to fire: face the vehicle
    else
        return;                                          // otherwise keep current facing

    m_sprite.setScale({ flip * scale, scale });
}

void Enemy::decideState() {
    if (isDroneInRange())
        m_state = EnemyState::Panicking;
    else if (isInFiringRange())
        m_state = EnemyState::Firing;
    else
        m_state = EnemyState::Advancing;
}

void Enemy::advance(float dt) {
    float direction = (m_vehicleX < getPosition().x) ? -1.f : 1.f;
    m_velocity.x = direction * getSpeed();
    move(dt);
}

void Enemy::panic(float dt) {
    float x = getPosition().x;

    // Get the movement bounds, then how far we are from each.
    auto [leftWall, rightWall] = calculateWalls(x);
    float distToLeft = x - leftWall;
    float distToRight = rightWall - x;

    // Update the U-turn state.
    updateUTurnState(distToLeft, distToRight);

    // If not mid-U-turn, flee away from the drone.
    if (!m_isUTurning)
        m_fleeDirection = (m_droneX < x) ? 1.f : -1.f;

    // Move and clamp inside the bounds.
    m_velocity.x = m_fleeDirection * getSpeed() * EnemyConfig::PanicMultiplier;
    move(dt);
    setPosition({ std::clamp(getPosition().x, leftWall, rightWall), getPosition().y });
}

// Computes the left/right movement bounds for the enemy's center,
// inset by its own half-width so the whole body stays inside the bounds.
std::pair<float, float> Enemy::calculateWalls(float x) const {
    float half = getBounds().size.x / 2.f;
    float gap = m_vehicleHalfWidth + EnemyConfig::VehicleGap;
    bool leftOfVehicle = (x < m_vehicleX);
    float leftWall = leftOfVehicle ? half : m_vehicleX + gap + half;
    float rightWall = leftOfVehicle ? m_vehicleX - gap - half : WindowConfig::Width - half;
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

void Enemy::updateFiring(float dt) {
    m_velocity.x = 0.f;
    m_fireTimer += dt;
    if (m_fireTimer >= EnemyConfig::TelegraphTime) {
        m_fireRequested = true;
        m_fireTimer = 0.f;
    }
}

// Hysteretic panic trigger: enter at PanicRange, leave only past PanicExitRange.
bool Enemy::isDroneInRange() const {
    float range = (m_state == EnemyState::Panicking) ? EnemyConfig::PanicExitRange
        : EnemyConfig::PanicRange;
    return std::abs(m_droneX - getPosition().x) < range;
}

bool Enemy::isInFiringRange() const {
    float centerDist = std::abs(m_vehicleX - getPosition().x);
    float gap = centerDist - m_vehicleHalfWidth - getBounds().size.x / 2.f;
    return gap <= m_stopRange;
}