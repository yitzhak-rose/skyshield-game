#pragma once
#include "Gameplay/Objects/MovingObject.h"
#include "Graphics/Animation.h"
#include "Enums/EnemyState.h"

/// @brief «abstract» base for all enemies. Holds the shared AI; subclasses set the params.
class Enemy : public MovingObject {
public:

    /// @brief Constructs an enemy with a texture, position, animation params, and muzzle offset.
    Enemy(const sf::Texture& texture, sf::Vector2f position,
        int frameCount, float frameDuration, float scale,
        sf::Vector2f muzzleOffset, GameState& gameState);

    /// @brief Updates the enemy for one frame (runs the AI: panic / fire / advance).
    void update(float dt) override;

    /// @brief Returns the world position the enemy's gun fires from (faces the vehicle).
    sf::Vector2f getMuzzlePosition() const;

    /// @brief Returns the bullet's launch velocity: direction toward the vehicle × bullet speed.
    float getBulletVelocityX() const;

    /// @brief Receives the vehicle's x-position from the Level.
    void setVehicleX(float vehicleX) { m_vehicleX = vehicleX; }

    /// @brief Receives the vehicle's half-width so ranges respect its body, not its center.
    void setVehicleHalfWidth(float halfWidth) { m_vehicleHalfWidth = halfWidth; }

    /// @brief Receives the drone's x-position from the Level.
    void setDroneX(float droneX) { m_droneX = droneX; }

    /// @brief Returns true if the enemy wants to fire a bullet this frame.
    bool isFireRequested() const { return m_fireRequested; }

    /// @brief Clears the fire request after the Level has created the bullet.
    void clearFireRequest() { m_fireRequested = false; }

    /// @brief Reacts to being hit by a bomb (FastEnemy dies, TankEnemy takes two hits).
    virtual void onHit() = 0;

    /// @brief Returns the coins awarded when this enemy is killed.
    virtual int getReward() const = 0;

protected:

    /// @brief Returns this enemy's base movement speed (subclasses differ).
    virtual float getSpeed() const = 0;

    /// @brief Advances the sprite animation for this frame (subclasses may override).
    virtual void updateAnimation(float dt);

    /// @brief Returns true while the enemy is in its firing state.
    bool isFiring() const { return m_state == EnemyState::Firing; }

    /// @brief Marks the enemy dead and records its reward in the shared state.
    void defeat();

    /// @brief Cycles the enemy's sprite-sheet frames.
    Animation m_animation;

private:

    /// @brief Returns -1 when the vehicle is to the left, +1 when to the right.
    float directionToVehicle() const;

    /// @brief Decides which state the enemy is in this frame.
    void decideState();

    /// @brief Moves the enemy toward the vehicle.
    void advance(float dt);

    /// @brief Moves the enemy away from the drone, clamped to the screen edges.
    void panic(float dt);

    /// @brief Computes the left/right movement bounds (screen edges + vehicle).
    std::pair<float, float> calculateWalls(float x) const;

    /// @brief Updates whether the enemy is mid-U-turn.
    void updateUTurnState(float distToLeft, float distToRight);

    /// @brief Holds position, counts the telegraph timer, requests a shot when ready.
    void updateFiring(float dt);

    /// @brief Returns true if the drone is within panic range.
    bool isDroneInRange() const;

    /// @brief Returns true if the enemy has reached its firing distance from the vehicle.
    bool isInFiringRange() const;

    /// @brief Faces the sprite toward its current movement direction.
    void updateFacing();

    /// @brief The enemy's current AI state.
    EnemyState m_state = EnemyState::Advancing;

    /// @brief The vehicle's x-position, supplied by the Level.
    float m_vehicleX = 0.f;

    /// @brief The vehicle's half-width, so ranges measure from its edge not its center.
    float m_vehicleHalfWidth = 0.f;

    /// @brief The drone's x-position, supplied by the Level.
    float m_droneX = 0.f;

    /// @brief This enemy's randomized stopping distance from the vehicle.
    float m_stopRange = 0.f;

    /// @brief Gun-tip offset from the enemy's center, defined facing right.
    sf::Vector2f m_muzzleOffset;

    /// @brief Time counted toward the next shot.
    float m_fireTimer = 0.f;

    /// @brief True for the one frame a shot should be created.
    bool  m_fireRequested = false;

    /// @brief Current flee direction while panicking (-1 left, +1 right).
    float m_fleeDirection = 0.f;

    /// @brief True while committed to a U-turn.
    bool  m_isUTurning = false;
};