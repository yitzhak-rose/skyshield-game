#pragma once
#include <SFML/Graphics.hpp>

class GameState;

/// \brief A drawable component showing game status (Level, Coins, Lives).
class MenuBar : public sf::Drawable, sf::Transformable {
public:
    /// \brief Constructs the MenuBar.
    /// \param width Width of the bar.
    /// \param height Height of the bar.
    /// \param state Reference to the game state.
    /// \param font Reference to the font to be used.
    MenuBar(float width, float height, GameState& state, const sf::Font& font);

    sf::Text displayText(std::string type, int val, sf::Vector2f pos, const sf::Font& font) const;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    mutable sf::RectangleShape m_background;

    const GameState& m_state;
    const sf::Font& m_font;

    /// \brief Draws the menu bar information (level, coins, lives).
    /// \param target The render target.
    /// \param states The render states.
    void drawMenuBarInfo(sf::RenderTarget& target, sf::RenderStates states) const;
};
