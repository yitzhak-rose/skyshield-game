#pragma once

#include "Gameplay/Objects/GameObject.h"
#include "Graphics/Animation.h"

/// @brief A battery indicator that selects a frame according to its charge percentage.
class Battery : public GameObject {
public:
    /// @brief Constructs a full battery indicator from a horizontal sprite sheet.
    Battery(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Updates the battery frame from the current battery percentage.
    void update(float dt) override;

    /// @brief Draws the battery on any render target, including the menu bar target.
    void draw(sf::RenderTarget& target, sf::RenderStates states = {}) const;

    /// @brief Scales the battery to a target visual height while preserving aspect ratio.
    void setDisplayHeight(float height);

    /// @brief Returns the current charge percentage.
    float getBatteryPercent() const;

    using GameObject::draw;

private:
    /// @brief Applies the animation frame that matches the current battery percentage.
    void updateFrame();

    /// @brief Returns the sprite-sheet frame for the current percentage.
    int getFrameIndex() const;

    /// @brief Provides the frame geometry of the battery sprite sheet.
    Animation m_animation;

};
