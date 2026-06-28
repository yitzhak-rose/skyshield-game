#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Interfaces/IDrawable.h"
#include "Interfaces/IUpdatable.h"

class GameState;

/// @brief «abstract» base of every game object. It is the single polymorphic
///        type the World stores, updates, draws, and collides — so it carries
///        the full contract: drawable (IDrawable) and updatable (IUpdatable).
class GameObject : public IDrawable, public IUpdatable {
public:

    /// @brief Constructs the object with a texture and a starting position.
    GameObject(const sf::Texture& texture, sf::Vector2f position, GameState& gameState);

    /// @brief Pure-virtual destructor.
    virtual ~GameObject() = 0;

    /// @brief Draws the object's sprite to the window.
    void draw(sf::RenderWindow& target) const override;

    /// @brief Hands the World an object this one wants to spawn this frame
    ///        (e.g. an enemy's bullet). Returns nullptr when there is nothing
    ///        to spawn, which is the default for every object.
    virtual std::unique_ptr<GameObject> takeSpawn() { return nullptr; }

    /// @brief Returns the object's collision box.
    virtual sf::FloatRect getBounds() const;

    /// @brief Returns the object's position.
    sf::Vector2f getPosition() const;

    /// @brief Sets the object's position.
    void setPosition(sf::Vector2f position);

    /// @brief Returns true once the object has been marked dead.
    bool isDead() const { return !m_alive; }

    /// @brief Marks the object as dead.
    void destroy() { m_alive = false; }

    /// @brief Returns true if this object represents the player (receives input).
    virtual bool isPlayer() const { return false; }

    /// @brief Returns true if this object is a target enemies should aim at.
    virtual bool isTarget() const { return false; }

protected:

    /// @brief Crops the sprite to one frame, centers its origin, and applies a scale.
    void setupSprite(sf::IntRect frame, float scale);

    /// @brief Returns mutable access to the shared game state.
    GameState& gameState() { return m_gameState; }

    /// @brief Returns read-only access to the shared game state.
    const GameState& gameState() const { return m_gameState; }

    /// @brief The object's drawable sprite.
    sf::Sprite m_sprite;

private:

    /// @brief Reference to the shared game state.
    GameState& m_gameState;

    /// @brief Tracks whether the object is still in play.
    bool m_alive = true;
};