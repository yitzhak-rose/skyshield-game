#include "GameObject.h"

// Constructor
GameObject::GameObject(const sf::Texture& texture, sf::Vector2f position)
    : m_sprite(texture)
{
    m_sprite.setPosition(position);
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


