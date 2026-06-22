#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "Interfaces/IDrawable.h"

class GameState;

/// @brief «abstract» base holding the shared data and the shared sprite-draw of every game object.
class GameObject : public IDrawable {
public:

    /// @brief Constructs the object with a texture and a starting position.
    GameObject(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Pure-virtual destructor.
    virtual ~GameObject() = 0;

    /// @brief Draws the object's sprite to the window.
    void draw(sf::RenderTarget& target) const override;

    /// @brief Returns the object's collision box.
    virtual sf::FloatRect getBounds() const;

    /// @brief Returns the object's position.
    sf::Vector2f getPosition() const;

    /// @brief Sets the object's position.
    void setPosition(sf::Vector2f position);

    /// @brief Returns true while the object is still in play.
    bool isAlive() const { return m_alive; }

    /// @brief Returns true once the object has been marked dead.
    bool isDead() const { return !m_alive; }

    /// @brief Marks the object as dead.
    void destroy() { m_alive = false; }

protected:

    /// @brief Crops the sprite to one frame, centers its origin, and applies a scale.
    void setupSprite(sf::IntRect frame, float scale);

    /// @brief Returns mutable access to the shared game state.
    GameState& gameState() { return m_gameState.get(); }

    /// @brief Returns read-only access to the shared game state.
    const GameState& gameState() const { return m_gameState.get(); }

    /// @brief The object's drawable sprite.
    sf::Sprite m_sprite;

private:

    /// @brief Reference to the shared game state.
    std::reference_wrapper<GameState> m_gameState;

    /// @brief Tracks whether the object is still in play.
    bool m_alive = true;
};
