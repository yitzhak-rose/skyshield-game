#include "Gameplay/Objects/GameObject.h"

// Constructor
GameObject::GameObject(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : m_sprite(texture)
    , m_gameState(gameState)
{
    m_sprite.setPosition(position);
}

// Crops the sprite to one frame, centers its origin, and applies a scale.
void GameObject::setupSprite(sf::IntRect frame, float scale) {
    m_sprite.setTextureRect(frame);
    m_sprite.setOrigin({ frame.size.x / 2.f, frame.size.y / 2.f });
    m_sprite.setScale({ scale, scale });
}

void GameObject::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}

// The sprite's on-screen rect (after position/scale) is our hitbox.
sf::FloatRect GameObject::getBounds() const {
    return m_sprite.getGlobalBounds();
}

// Position getter.
sf::Vector2f GameObject::getPosition() const {
    return m_sprite.getPosition();
}

// Position setter.
void GameObject::setPosition(sf::Vector2f position) {
    m_sprite.setPosition(position);
}
