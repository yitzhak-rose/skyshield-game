#pragma once
#include "Gameplay/Objects/GameObject.h"
#include "Graphics/Animation.h"

/// @brief A menu-bar battery indicator that selects a frame according to its charge percentage.
class Battery : public GameObject {
public:
    /// @brief Constructs a full battery indicator from a horizontal sprite sheet.
    Battery(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Draws the battery on a render target (the menu bar).
    void draw(sf::RenderTarget& target, sf::RenderStates states = {}) const;

    /// @brief Returns the current charge percentage.
    float getBatteryPercent() const;

private:
    /// @brief Returns the sprite-sheet frame for the current percentage.
    int getFrameIndex() const;

    /// @brief Provides the frame geometry of the battery sprite sheet.
    Animation m_animation;
};