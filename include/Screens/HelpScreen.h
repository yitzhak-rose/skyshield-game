#pragma once

#include "Enums/GameObjects.h"
#include "Screens/Core/Screen.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class HelpScreen : public Screen {
public:
    HelpScreen();

    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;
    void draw(sf::RenderWindow& target) override;

private:
    void configureLayout();
    void configureBackground();
    void handleClick(sf::Vector2f position);
    void updateHover(sf::Vector2f position);

    sf::FloatRect backButtonBounds() const;
    void drawBackground(sf::RenderTarget& target) const;
    void drawBackButton(sf::RenderTarget& target) const;
    void drawPanel(sf::RenderTarget& target) const;
    void drawInstructions(sf::RenderTarget& target) const;
    void drawIconRow(sf::RenderTarget& target,
                     GameObjects icon,
                     const std::string& text,
                     sf::Vector2f position,
                     float iconSize) const;
    void drawMovementRow(sf::RenderTarget& target, sf::Vector2f position) const;
    void drawSpacebarRow(sf::RenderTarget& target, sf::Vector2f position) const;
    void drawSectionHeading(sf::RenderTarget& target,
                            const std::string& title,
                            sf::Vector2f position,
                            sf::Color color) const;
    void drawOutcomeCard(sf::RenderTarget& target,
                         const std::string& title,
                         const std::vector<std::string>& bodyLines,
                         sf::FloatRect bounds,
                         sf::Color titleColor) const;
    void drawLoseConditions(sf::RenderTarget& target, sf::FloatRect bounds) const;

    sf::Sprite m_backgroundSprite;
    sf::RectangleShape m_dim;
    sf::RectangleShape m_panel;
    sf::Sprite m_backSprite;
    const sf::Font& m_font;
    bool m_backHovered = false;
};
