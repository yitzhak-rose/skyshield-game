#pragma once
#include "Gameplay/Objects/MovingObject.h"
#include "Graphics/Animation.h"
#include "Gameplay/Constants.h"

/// @brief The player-controlled drone. Moves horizontally, drops bombs, manages resources.
class Drone : public MovingObject
{
public:
    /// @brief Constructs the drone with a texture and a starting position.
    Drone(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Updates the drone for one frame (movement, animation, battery drain).
    void update(float dt) override;

    /// @brief Starts moving the drone left.
    void moveLeft();

    /// @brief Starts moving the drone right.
    void moveRight();

    /// @brief Stops the drone's horizontal movement.
    void stop();

    /// @brief Consumes one bomb from the ammo count.
    bool consumeAmmo();

private:
    /// @brief Cycles the drone's rotor frames.
    Animation m_animation;
};