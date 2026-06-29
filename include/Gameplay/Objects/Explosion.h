#pragma once
#include "Gameplay/Objects/GameObject.h"
#include "Graphics/Animation.h"
#include "App/Constants.h"

/// @brief A one-shot visual effect: plays the explosion animation once, then dies.
class Explosion : public GameObject {
public:
    /// @brief Builds an explosion centered at a position.
    Explosion(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Advances the animation; destroys itself after the last frame.
    void update(float dt) override;

    /// @brief Explosions don't collide with anything.
    sf::FloatRect getBounds() const override { return {}; }

private:
    /// @brief Frame geometry of the explosion sheet.
    Animation m_animation;

    /// @brief Time accumulated to drive the one-shot playback.
    float m_timer = 0.f;
};