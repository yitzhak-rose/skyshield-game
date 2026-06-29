#pragma once
#include "Gameplay/Objects/GameObject.h"
#include "Graphics/Animation.h"
#include "Gameplay/Constants.h"

/// @brief The armored command vehicle the player protects. Stays in place but animates each frame.
class CommandVehicle : public GameObject
{
public:
    /// @brief Constructs the vehicle with a texture and a starting position.
    CommandVehicle(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Updates the vehicle for one frame (loops beacon frames, or burning frames if critical).
    void update(float dt) override;

    /// @brief Reduces the armor by one hit's worth of damage.
    void takeDamage();

    /// @brief Returns the current armor as a percentage (0 to 100).
    float getArmorPercent() const;

    /// @brief Marks the vehicle as a target enemies should advance on.
    bool isTarget() const override { return true; }

private:
    /// @brief Picks and shows the right beacon/burning frame for this moment.
    void updateAnimationFrame(float dt);

    /// @brief Drives the post-hit shake offset, or rests at the base position when idle.
    void updateShake(float dt);

    /// @brief Provides the frame geometry of the vehicle sheet.
    Animation m_animation;

    /// @brief The vehicle's resting position; the sprite returns here after each shake.
    sf::Vector2f m_basePosition;

    /// @brief Time accumulated to drive the frame loop.
    float m_frameTimer = 0.f;

    /// @brief Time left in the current hit-shake (0 = not shaking).
    float m_shakeTimer = 0.f;
};