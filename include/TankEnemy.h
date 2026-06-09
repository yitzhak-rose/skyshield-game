#pragma once
#include "Enemy.h"

// A slow, armored enemy: takes two hits — the first only wounds it.
class TankEnemy : public Enemy {
public:

    // Constructs a tank enemy with a texture and a starting position.
    TankEnemy(const sf::Texture& texture, sf::Vector2f position);

    // First hit wounds (changes color); second hit kills.
    void onHit() override;

    // Returns the coins awarded for killing this enemy.
    int getReward() const override;

protected:

    // Returns this enemy's movement speed.
    float getSpeed() const override;

private:
    bool m_wounded = false; // true after the first hit
};