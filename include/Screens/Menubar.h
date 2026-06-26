#pragma once
#include <SFML/Graphics.hpp>
#include "Gameplay/Coin.h"

class GameState;

/// \brief A drawable component showing the player's bombs, coins, vehicle armor, title, and battery status.
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
    const sf::Font& m_levelFont;

    float m_width;
    float m_height;

    void configureBackground();
    void configureBombSprite();
    void configureBatterySprite();
    void configureCoin();
    void drawContent(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawBombCounter(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Text makeBombCounterText() const;
    void drawTitle(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Text makeTitleText() const;
    void drawVehicleArmorBar(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Vector2f vehicleArmorBarPosition() const;
    sf::RectangleShape makeVehicleArmorTrack() const;
    sf::RectangleShape makeVehicleArmorFill(float armorPercent) const;
    sf::RectangleShape makeVehicleArmorOutline() const;
    sf::Color vehicleArmorFillColor(float armorPercent) const;
    sf::Text makeVehicleArmorText(float armorPercent) const;
    void drawBattery(sf::RenderTarget& target, sf::RenderStates states) const;
    void updateBatterySpriteForView(const sf::View& view) const;
    sf::IntRect batteryFrameRect() const;
    void drawLevelIndicator(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Text makeLevelText() const;
};
