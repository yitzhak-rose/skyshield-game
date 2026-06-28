#include "Gameplay/Coin.h"
#include "Gameplay/Constants.h"
#include "Utilities/Utils.h"
#include <string>

// Crops the sprite to one spin frame and centers its origin so it rotates in place.
Coin::Coin(const sf::Texture& texture, const sf::Font& font)
    : m_animation(Animation::frameSize(texture, CoinConfig::Sprite.frameCount),
        CoinConfig::Sprite.frameCount, CoinConfig::Sprite.frameDuration)
    , m_sprite(texture)
    , m_font(font)
{
    m_sprite.setTextureRect(m_animation.getFrameRect());
    GraphicsUtils::centerOrigin(m_sprite);
    m_sprite.setScale({ CoinConfig::Sprite.scale, CoinConfig::Sprite.scale });
}

// Spins the icon every frame; the pop timer (if any) counts down toward rest.
void Coin::update(float dt) {
    m_animation.apply(m_sprite, dt);

    if (m_popTimer > 0.f)
        m_popTimer = std::max(0.f, m_popTimer - dt);
}

// Only a rise pops — spending coins (a drop) just updates the number quietly.
void Coin::setCount(int count) {
    if (count > m_count)
        m_popTimer = CoinConfig::PopDuration;
    m_count = count;
}

void Coin::setPosition(sf::Vector2f position) {
    m_sprite.setPosition(position);
}

// Draws the spinning icon, then the count to its right, scaled up mid-pop.
void Coin::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, states);

    sf::Text text(m_font, std::to_string(m_count), MenuBarConfig::TextSize);
    text.setFillColor(sf::Color::White);

    // Pop curve: a half-sine over the timer, so the number swells then settles.
    const float t = m_popTimer / CoinConfig::PopDuration;        // 1 -> 0
    const float popScale = 1.f + std::sin(t * 3.14159265f) * CoinConfig::PopMagnitude;

    const sf::Vector2f iconPos = m_sprite.getPosition();
    const float iconHalfWidth = m_sprite.getGlobalBounds().size.x / 2.f;

    // Anchor the text on its left-center so the pop grows outward, not sideways.
    GraphicsUtils::centerOrigin(text);
    text.setScale({ popScale, popScale });
    text.setPosition({ iconPos.x + iconHalfWidth + MenuBarConfig::BombTextGap
                       + text.getGlobalBounds().size.x / 2.f, iconPos.y });

    target.draw(text, states);
}
