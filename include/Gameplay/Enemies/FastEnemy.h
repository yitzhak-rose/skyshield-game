#pragma once
#include "Gameplay/Enemies/Enemy.h"

/// @brief A fast, fragile enemy: dies from a single bomb hit.
class FastEnemy : public Enemy {
public:

    /// @brief Constructs a fast enemy with a texture and a starting position.
    FastEnemy(const sf::Texture& texture, sf::Vector2f position);

    /// @brief Dies immediately when hit.
    void onHit() override;

    /// @brief Returns the coins awarded for killing this enemy.
    int getReward() const override;

private:

    /// @brief Returns this enemy's movement speed.
    float getSpeed() const override;
};
