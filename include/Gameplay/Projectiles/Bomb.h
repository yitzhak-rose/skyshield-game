#pragma once
#include "Gameplay/Objects/MovingObject.h"
#include "Graphics/Animation.h"

/// @brief A bomb dropped by the drone. Falls straight down; dies on hit or off-screen.
class Bomb : public MovingObject
{
public:
    /// @brief Constructs a bomb at a position with a fixed downward velocity.
    Bomb(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Moves the bomb down, animates it, and destroys it once off-screen.
    void update(float dt) override;

private:
    /// @brief Cycles the bomb's wobble frames.
    Animation m_animation;
};