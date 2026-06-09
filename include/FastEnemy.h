#pragma once
#include "Enemy.h"

// A fast, fragile enemy: dies from a single bomb hit.
class FastEnemy : public Enemy {
public:

    // Constructs a fast enemy with a texture and a starting position.
    FastEnemy(const sf::Texture& texture, sf::Vector2f position);

    // Dies immediately when hit.
    void onHit() override;

    // Returns the coins awarded for killing this enemy.
    int getReward() const override;

protected:

    // Returns this enemy's movement speed.
    float getSpeed() const override;
};