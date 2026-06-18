#pragma once
#include <SFML/Graphics.hpp>
#include "Graphics/Animation.h"

/// @brief A HUD widget: a spinning coin icon plus the player's coin count.
class Coin {
public:
    /// @brief Builds the widget from the coin sprite sheet and a font for the count.
    Coin(const sf::Texture& texture, const sf::Font& font);

    /// @brief Advances the spin animation and decays any active count "pop".
    void update(float dt);

    /// @brief Feeds the latest coin total; a rise triggers a short pop on the number.
    void setCount(int count);

    /// @brief Places the icon's center; the number is drawn just to its right.
    void setPosition(sf::Vector2f position);

    /// @brief Draws the icon and the count onto the menu bar.
    void draw(sf::RenderTarget& target, sf::RenderStates states = {}) const;

private:
    /// @brief Cycles the coin's spin frames.
    Animation m_animation;

    /// @brief The spinning coin icon.
    sf::Sprite m_sprite;

    /// @brief The font used for the count text.
    const sf::Font& m_font;

    /// @brief The coin total currently being displayed.
    int m_count = 0;

    /// @brief Time left in the current pop (0 = number is at rest).
    float m_popTimer = 0.f;
};