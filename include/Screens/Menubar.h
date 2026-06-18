#pragma once
#include <SFML/Graphics.hpp>
#include "Gameplay/Coin.h"

class GameState;

/// \brief A drawable component showing the player's bombs, coins, title, and battery status.
class MenuBar : public sf::Drawable, sf::Transformable {
public:
    /// @brief Constructs the MenuBar.
    /// @param height Height of the bar.
    /// @param state Reference to the game state.
    MenuBar(float height, GameState& state);

    /// @brief Advances time-based content (the coin's spin and count pop).
    void update(float dt);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    mutable sf::RectangleShape m_background;
    sf::Sprite m_bombSprite;
    mutable sf::Sprite m_batterySprite;
    Coin m_coin;

    const GameState& m_state;
    const sf::Font& m_font;

    float m_width;
    float m_height;

    void configureBackground();
    void configureBombSprite();
    void configureBatterySprite();
    void configureCoin();
    void drawContent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawBombCounter(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawTitle(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawBattery(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Text makeBombCounterText() const;
    sf::Text makeTitleText() const;
    sf::IntRect batteryFrameRect() const;
    void updateBatterySpriteForView(const sf::View& view) const;
};