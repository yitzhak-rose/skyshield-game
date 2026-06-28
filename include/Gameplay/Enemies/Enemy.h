#pragma once
#include "Gameplay/Objects/MovingObject.h"
#include "Graphics/Animation.h"
#include "Enums/EnemyState.h"
#include <memory>

struct EnemyTypeConfig;

/// @brief «abstract» base for all enemies. Holds the shared AI; subclasses set the params.
///        Reads the drone and the vehicle live through references, so there is no
///        per-frame plumbing of their positions from the World.
class Enemy : public MovingObject {
public:

    /// @brief Constructs an enemy with a texture, position, type config, and live target references.
    Enemy(const sf::Texture& texture, sf::Vector2f position,
        const EnemyTypeConfig& stats, GameState& gameState,
        const GameObject& player, const GameObject& target);

    /// @brief Updates the enemy for one frame (runs the AI: panic / fire / advance).
    void update(float dt) override;

    /// @brief When the enemy has decided to fire, hands the World a freshly built bullet.
    std::unique_ptr<GameObject> takeSpawn() override;

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

    /// @brief Returns true if the enemy wants to fire a bullet this frame.
    bool isFireRequested() const { return m_fireRequested; }

    /// @brief Marks the enemy dead and records its reward in the shared state.
    void defeat();

    /// @brief Cycles the enemy's sprite-sheet frames.
    Animation m_animation;

private:

    /// @brief Returns the world position the enemy's gun fires from (faces the vehicle).
    sf::Vector2f getMuzzlePosition() const;

    /// @brief Returns the bullet's launch velocity: direction toward the vehicle × bullet speed.
    float getBulletVelocityX() const;

    /// @brief Returns the vehicle's current x-position.
    float targetX() const;

    /// @brief Returns the vehicle's current half-width (measured from its center).
    float targetHalfWidth() const;

    /// @brief Returns the drone's current x-position.
    float playerX() const;

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

    /// @brief The drone the enemy flees from (read live, never owned).
    const GameObject& m_player;

    /// @brief The vehicle the enemy advances on and fires at (read live, never owned).
    const GameObject& m_target;

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

    /// @brief True for the one frame an explosion should be spawned (set on death).
    bool m_explosionRequested = false;
};
