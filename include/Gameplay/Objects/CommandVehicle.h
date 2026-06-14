#pragma once
#include "Gameplay/Objects/GameObject.h"
#include "Graphics/Animation.h"
#include "Gameplay/Constants.h"

/// @brief The armored command vehicle the player protects. Static (inherits GameObject).
class CommandVehicle : public GameObject
{
public:
    /// @brief Constructs the vehicle with a texture and a starting position.
    CommandVehicle(const sf::Texture& texture, sf::Vector2f position);

    /// @brief Updates the vehicle for one frame (loops beacon frames, or burning frames if critical).
    void update(float dt) override;

    /// @brief Reduces the armor by one hit's worth of damage.
    void takeDamage();

    /// @brief Returns the current armor as a percentage (0 to 100).
    float getArmorPercent() const { return m_armor; }

private:
    /// @brief Provides the frame geometry of the vehicle sheet.
    Animation m_animation;

    /// @brief Time accumulated to drive the frame loop.
    float m_frameTimer = 0.f;

    /// @brief Remaining armor as a percentage.
    float m_armor = VehicleConfig::StartArmor;
};
