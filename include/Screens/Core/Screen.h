#pragma once

#include "Enums/ScreenCommand.h"
#include "Enums/LevelResult.h"
#include "Screens/Components/OverlayView.h"
#include "Structs/ScreenEventContext.h"

#include <SFML/Graphics.hpp>
#include <initializer_list>
#include <memory>
#include <string>

/// \brief Abstract base class for all game screens.
class Screen {
public:
    /// @brief Default constructor.
    Screen();

    /// @brief Virtual destructor.
    virtual ~Screen() = 0;

    /// @brief Draws the screen content.
    /// @param target The render target.
    virtual void draw(sf::RenderWindow& target) = 0;

    /// @brief Routes one event to an overlay or this screen.
    /// @param event The event to handle.
    /// @param context Event data derived by Game.
    void handleEvent(const sf::Event& event, const ScreenEventContext& context);

    /// @brief Returns true when the event is a mouse-move event.
    static bool isMouseMoved(const sf::Event& event);

    /// @brief Returns true when the event is a left mouse-button press.
    static bool isLeftMousePressed(const sf::Event& event);

    /// @brief Returns true when the event is a press of the requested key.
    static bool isKeyPressed(const sf::Event& event, sf::Keyboard::Key key);

    /// @brief Returns true when the event is a press of any requested key.
    static bool isAnyKeyPressed(const sf::Event& event, std::initializer_list<sf::Keyboard::Key> keys);

    /// @brief Updates the screen logic.
    /// @param deltaTime Time elapsed since last frame.
    virtual void update(float /*deltaTime*/) {}

    /// @brief Returns the command requested by this screen.
    ScreenCommand getCommand() const { return m_command; }

    /// @brief Clears the pending command after Game handles it.
    void clearCommand() { m_command = ScreenCommand::None; }

    /// @brief Returns the center point of a rectangle.
    static sf::Vector2f centerOf(sf::FloatRect bounds);

    /// @brief Returns the center point of a rectangle shape's global bounds.
    static sf::Vector2f centerOf(const sf::RectangleShape& shape);

    /// @brief Scales and centers a sprite inside a rectangle.
    static void fitSpriteTo(sf::Sprite& sprite, sf::FloatRect bounds);

    /// @brief Applies fill, outline color, and outline thickness to a rectangle.
    static void styleRectangle(sf::RectangleShape& shape,
                               sf::Color fill,
                               sf::Color outline,
                               float outlineThickness);

    /// @brief Creates styled text at a position.
    static sf::Text makeText(const sf::Font& font,
                             const std::string& value,
                             unsigned int size,
                             sf::Vector2f position,
                             sf::Color color);

    /// @brief Creates styled text with its origin centered.
    static sf::Text makeCenteredText(const sf::Font& font,
                                     const std::string& value,
                                     unsigned int size,
                                     sf::Vector2f position,
                                     sf::Color color);

protected:
    /// @brief Handles one event after overlay routing.
    virtual void handleScreenEvent(const sf::Event& /*event*/, const ScreenEventContext& /*context*/) {}

    /// @brief Requests an app-level command for Game to execute.
    void requestCommand(ScreenCommand command);

    /// @brief Shows an overlay owned by this screen.
    void showOverlay(std::unique_ptr<OverlayView> overlay);

    /// @brief Returns true while this screen has an active overlay.
    bool hasOverlay() const { return m_overlay != nullptr; }

    /// @brief Sends an event to the overlay when one is active.
    bool handleOverlayEvent(const sf::Event& event, const ScreenEventContext& context);

    /// @brief Updates the overlay when one is active.
    bool updateOverlay(float deltaTime);

    /// @brief Draws the overlay above this screen's content.
    void drawOverlay(sf::RenderWindow& target) const;

    /// @brief Removes the overlay after it dismisses itself.
    void clearDismissedOverlay();

private:
    std::unique_ptr<OverlayView> m_overlay;
    ScreenCommand m_command = ScreenCommand::None;
};
