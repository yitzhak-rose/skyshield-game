#pragma once
#include "MovingObject.h"
#include "Enums/EnemyState.h"

// «abstract» base for all enemies. Holds the shared AI; subclasses set the params.
class Enemy : public MovingObject {
public:
    
    // Constructs an enemy with a texture and a starting position.
    Enemy(const sf::Texture& texture, sf::Vector2f position);

    // Updates the enemy for one frame (runs the AI: panic / fire / advance).
    void update(float dt) override;

    // Receives the vehicle's x-position from the Level.
    void setVehicleX(float vehicleX) { m_vehicleX = vehicleX; }

    // Receives the drone's x-position from the Level.
    void setDroneX(float droneX) { m_droneX = droneX; }

    // Returns true if the enemy wants to fire a bullet this frame.
    bool isFireRequested() const { return m_fireRequested; }

    // Clears the fire request after the Level has created the bullet.
    void clearFireRequest() { m_fireRequested = false; }

    // Reacts to being hit by a bomb (FastEnemy dies, TankEnemy takes two hits).
    virtual void onHit() = 0;

    // Returns the coins awarded when this enemy is killed.
    virtual int getReward() const = 0;

protected:

    // Returns this enemy's base movement speed (subclasses differ).
    virtual float getSpeed() const = 0;

private:

    // Decides which state the enemy is in this frame.
    void decideState();

    // Moves the enemy toward the vehicle.
    void advance(float dt);

    // Moves the enemy away from the drone, clamped to the screen edges.
    void panic(float dt);
    std::pair<float, float> calculateWalls(float x) const;
    void updateUTurnState(float distToLeft, float distToRight);

    // Holds position, counts the telegraph timer, requests a shot when ready.
    void updateFiring(float dt);

    // Returns true if the drone is within panic range.
    bool isDroneInRange() const;

    // Returns true if the enemy has reached its firing distance from the vehicle.
    bool isInFiringRange() const;

    EnemyState m_state = EnemyState::Advancing;

    float m_vehicleX = 0.f;
    float m_droneX = 0.f;
    float m_stopRange = 0.f;
    float m_fireTimer = 0.f;
    bool  m_fireRequested = false;
    float m_fleeDirection = 0.f; 
    bool  m_isUTurning = false; // true while committed to a U-turn 

};