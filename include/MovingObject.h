#pragma once
#include "GameObject.h"

// «abstract» base for objects that move: adds velocity on top of GameObject.
class MovingObject : public GameObject {
public:

    // Constructs the object with a texture, position, and velocity.
    MovingObject(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f velocity);

protected:

    // Moves the object by its velocity for one frame.
    void move(float dt);

    // The object's velocity in pixels per second.
    sf::Vector2f m_velocity;
};