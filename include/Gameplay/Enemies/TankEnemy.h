#pragma once
#include "Gameplay/Enemies/Enemy.h"

/// @brief A slow, armored enemy: takes two hits — the first only wounds it.
class TankEnemy : public Enemy {
public:
    /// @brief Constructs a tank enemy with a texture, position, and the live drone/vehicle refs.
    TankEnemy(const sf::Texture& texture, sf::Vector2f position, GameState& gameState,
        const GameObject& player, const GameObject& target);

    /// @brief First hit wounds (changes color); second hit kills.
    void onHit() override;

    /// @brief Returns the coins awarded for killing this enemy.
    int getReward() const override;

private:
    /// @brief Returns this enemy's movement speed.
    float getSpeed() const override;

    /// @brief Plays the 5 firing frames across the telegraph window, in sync with the timer.
    void updateAnimation(float dt) override;

    /// @brief True after the first hit.
    bool m_wounded = false;
};