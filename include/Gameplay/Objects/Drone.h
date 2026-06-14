#pragma once
#include "Gameplay/Objects/MovingObject.h"
#include "Graphics/Animation.h"
#include "Gameplay/Constants.h"

/// @brief The player-controlled drone. Moves horizontally, drops bombs, manages resources.
class Drone : public MovingObject
{
public:
    /// @brief Constructs the drone with a texture and a starting position.
    Drone(const sf::Texture& texture, sf::Vector2f position);

    /// @brief Updates the drone for one frame (movement, animation, battery drain).
    void update(float dt) override;

    /// @brief Starts moving the drone left.
    void moveLeft();

    /// @brief Starts moving the drone right.
    void moveRight();

    /// @brief Stops the drone's horizontal movement.
    void stop();

    /// @brief Returns true if the drone has at least one bomb left.
    bool canDropBomb() const { return m_ammo > 0; }

    /// @brief Consumes one bomb from the ammo count.
    void consumeAmmo() { m_ammo--; }

    /// @brief Sets how fast the battery drains per second.
    void setDrainRate(float rate) { m_drainRate = rate; }

    /// @brief Sets the ammo count.
    void setAmmo(int ammo) { m_ammo = ammo; }

    /// @brief Returns the current battery level as a percentage (0 to 100).
    float getBatteryPercent() const { return m_battery; }

    /// @brief Returns the number of bombs remaining.
    int getRemainingAmmo() const { return m_ammo; }

private:
    /// @brief Cycles the drone's rotor frames.
    Animation m_animation;

    /// @brief Remaining battery charge as a percentage.
    float m_battery = DroneConfig::StartBattery;

    /// @brief How much battery drains per second.
    float m_drainRate = 0.f;

    /// @brief Number of bombs left to drop.
    int m_ammo = 0;
};
