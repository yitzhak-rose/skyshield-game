#pragma once

#include "Gameplay/Objects/GameObject.h"
#include "Graphics/Animation.h"

/// @brief A battery indicator that selects a frame according to its charge percentage.
class Battery : public GameObject {
public:
    /// @brief Constructs a full battery indicator from a horizontal sprite sheet.
    Battery(const sf::Texture& texture, sf::Vector2f position);

    /// @brief Keeps the GameObject interface; the battery has no time-based behavior.
    void update(float dt) override;

    /// @brief Draws the battery on any render target, including the menu bar target.
    void draw(sf::RenderTarget& target, sf::RenderStates states = {}) const;

    /// @brief Sets the charge percentage and updates the displayed frame.
    void setBatteryPercent(float percent);

    /// @brief Returns the current charge percentage.
    float getBatteryPercent() const;

    using GameObject::draw;

private:
    /// @brief Returns the sprite-sheet frame for the current percentage.
    int getFrameIndex() const;

    /// @brief Provides the frame geometry of the battery sprite sheet.
    Animation m_animation;

    /// @brief The displayed charge percentage, clamped to 0 through 100.
    float m_batteryPercent;
};
