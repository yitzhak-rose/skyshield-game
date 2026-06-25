#pragma once
#include "Gameplay/Objects/GameObject.h"

/// @brief «abstract» base for objects that move; adds velocity on top of GameObject.
///        (update() is already part of GameObject's contract, so it is not re-declared here.)
class MovingObject : public GameObject {
public:

    /// @brief Constructs the object with a texture, position, and velocity.
    MovingObject(const sf::Texture& texture, sf::Vector2f position,
        sf::Vector2f velocity, GameState& gameState);

protected:

    /// @brief Moves the object by its velocity for one frame.
    void move(float dt);

    /// @brief The object's velocity in pixels per second.
    sf::Vector2f m_velocity;
};