#pragma once
#include "Gameplay/Objects/MovingObject.h"
#include "Graphics/Animation.h"

// A bomb dropped by the drone. Falls straight down; dies on hit or off-screen.
class Bomb : public MovingObject
{
public:
    // Constructs a bomb at a position; velocity is fixed (straight down).
    Bomb(const sf::Texture& texture, sf::Vector2f position);

    // Moves the bomb down and destroys it once it leaves the screen.
    void update(float dt) override;

private:
    // Cycles the bomb's wobble frames.
    Animation m_animation;
};