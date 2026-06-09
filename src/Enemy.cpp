#include "Enemy.h"
#include "Constants.h"
#include <random>
#include <algorithm>
#include <cmath>

// Returns a random float between min and max.
static float randomFloat(float min, float max) {

    // Create the random engine once and reuse it for all calls.
    static std::mt19937 generator(std::random_device{}());

    // Define a rule that maps the engine's output evenly into [min, max].
    std::uniform_real_distribution<float> dist(min, max);

    // Ask the distribution for one number and return it.
    return dist(generator);
}

Enemy::Enemy(const sf::Texture& texture, sf::Vector2f position)
    : MovingObject(texture, position, { 0.f, 0.f })
    , m_stopRange(randomFloat(EnemyConfig::StopRangeMin, EnemyConfig::StopRangeMax))
{
}

void Enemy::update(float dt) {
    decideState();
    switch (m_state) {
    case EnemyState::Advancing:  advance(dt);       break;
    case EnemyState::Panicking:  panic(dt);         break;
    case EnemyState::Firing:     updateFiring(dt);  break;
    }
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

// Computes the left/right movement bounds (screen edges + vehicle).
std::pair<float, float> Enemy::calculateWalls(float x) const {
    float gap = EnemyConfig::VehicleGap;
    bool leftOfVehicle = (x < m_vehicleX);
    float leftWall = leftOfVehicle ? 0.f : m_vehicleX + gap;
    float rightWall = leftOfVehicle ? m_vehicleX - gap : WindowConfig::Width;
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

bool Enemy::isDroneInRange() const {
    return std::abs(m_droneX - getPosition().x) < EnemyConfig::PanicRange;
}

bool Enemy::isInFiringRange() const {
    return std::abs(m_vehicleX - getPosition().x) <= m_stopRange;
}