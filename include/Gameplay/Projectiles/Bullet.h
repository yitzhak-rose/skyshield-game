#pragma once
#include "Gameplay/Objects/MovingObject.h"

/// @brief A bullet fired by an enemy toward the command vehicle. Moves horizontally.
class Bullet : public MovingObject
{
public:
    /// @brief Constructs a bullet at a position moving at a given horizontal velocity.
    Bullet(const sf::Texture& texture, sf::Vector2f position,
        float velocityX, GameState& gameState);

    /// @brief Moves the bullet and destroys it once it leaves the screen.
    void update(float dt) override;
};