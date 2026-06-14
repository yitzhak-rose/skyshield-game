#pragma once
#include "Enemy.h"

/// @brief A slow, armored enemy: takes two hits — the first only wounds it.
class TankEnemy : public Enemy {
public:
    /// @brief Constructs a tank enemy with a texture and a starting position.
    TankEnemy(const sf::Texture& texture, sf::Vector2f position);

    /// @brief First hit wounds (changes color); second hit kills.
    void onHit() override;

    /// @brief Returns the coins awarded for killing this enemy.
    int getReward() const override;

private:
    /// @brief Returns this enemy's movement speed.
    float getSpeed() const override;

    /// @brief Animates the firing flash only while firing; stays on frame 0 otherwise.
    void updateAnimation(float dt) override;

    /// @brief True after the first hit.
    bool m_wounded = false;
};