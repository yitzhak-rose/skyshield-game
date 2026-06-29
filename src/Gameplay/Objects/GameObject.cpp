#include "Gameplay/Objects/GameObject.h"

// Stores the sprite and the game-state reference, then places the sprite.
GameObject::GameObject(const sf::Texture& texture, sf::Vector2f position, GameState& gameState)
    : m_sprite(texture)
    , m_gameState(gameState)
{
    m_sprite.setPosition(position);
}

GameObject::~GameObject() = default;

void GameObject::draw(sf::RenderWindow& target) const {
    target.draw(m_sprite);
}

void GameObject::setupSprite(sf::IntRect frame, float scale) {
    m_sprite.setOrigin({ frame.size.x / 2.f, frame.size.y / 2.f });
    m_sprite.setScale({ scale, scale });
}

sf::FloatRect GameObject::getBounds() const {
    return m_sprite.getGlobalBounds();
}

sf::Vector2f GameObject::getPosition() const {
    return m_sprite.getPosition();
}

void GameObject::setPosition(sf::Vector2f position) {
    m_sprite.setPosition(position);
}