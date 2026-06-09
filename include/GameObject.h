#pragma once
#include <SFML/Graphics.hpp>

// «abstract» base for every visible, collidable object.
class GameObject {
public:

    // Constructs the object with a texture and a starting position.
    GameObject(const sf::Texture& texture, sf::Vector2f position);

    // Destructor.
    virtual ~GameObject() = default;

    // Updates the object's state for one frame.
    virtual void update(float dt) = 0;

    // Draws the object to the window.
    virtual void draw(sf::RenderWindow& window) const;

    // Returns the object's collision box.
    virtual sf::FloatRect getBounds() const;

    // Returns the object's position.
    sf::Vector2f getPosition() const;

    // Sets the object's position.
    void setPosition(sf::Vector2f position);

    // Returns true while the object is still in play.
    bool isAlive() const { return m_alive; }

    // Marks the object as dead.
    void destroy() { m_alive = false; }

protected:

    // The object's drawable sprite.
    sf::Sprite m_sprite;

private:
    bool m_alive = true;
};