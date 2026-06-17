#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class GameState;

// «abstract» base for every visible, collidable object.
class GameObject {
public:

    // Constructs the object with a texture and a starting position.
    GameObject(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

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

    // Returns true if the object has been dead.
    bool isDead() const { return !m_alive; }

    // Marks the object as dead.
    void destroy() { m_alive = false; }

protected:

    // Crops the sprite to one frame, centers its origin, and applies a scale.
    void setupSprite(sf::IntRect frame, float scale);

    GameState& gameState() { return m_gameState.get(); }
    const GameState& gameState() const { return m_gameState.get(); }

    // The object's drawable sprite.
    sf::Sprite m_sprite;

private:
    std::reference_wrapper<GameState> m_gameState;
    bool m_alive = true;
};
