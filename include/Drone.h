#pragma once
#include "MovingObject.h"
#include "Constants.h"

// The player-controlled drone. Moves horizontally, drops bombs, manages resources.
class Drone : public MovingObject
{
public:
    // Constructs the drone with a texture and a starting position.
    Drone(const sf::Texture &texture, sf::Vector2f position);

    // Updates the drone for one frame (movement, battery drain).
    void update(float dt) override;

    // Starts moving the drone left.
    void moveLeft();

    // Starts moving the drone right.
    void moveRight();

    // Stops the drone's horizontal movement.
    void stop();

    // Returns true if the drone has at least one bomb left.
    bool canDropBomb() const { return m_ammo > 0; }

    // Consumes one bomb from the ammo count.
    void consumeAmmo() { m_ammo--; }

    // Sets how fast the battery drains per second.
    void setDrainRate(float rate) { m_drainRate = rate; }

    // Returns the current battery level as a percentage (0 to 100).
    float getBatteryPercent() const { return m_battery; }

    // Returns the number of bombs remaining.
    int getRemainingAmmo() const { return m_ammo; }

private:
    float m_battery = DroneConfig::StartBattery;
    float m_drainRate = DroneConfig::BatteryDrainRate;
    int m_ammo = DroneConfig::StartAmmo;
};