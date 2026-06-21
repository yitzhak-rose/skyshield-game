#pragma once
#include <SFML/Graphics.hpp>
#include "Enums/LevelResult.h"
#include "Screens/OverlayView.h"
#include <memory>

/// \brief Abstract base class for all game screens.
class Screen {
public:
    /// @brief Default constructor.
    Screen();

    /// @brief Virtual destructor.
    virtual ~Screen() = 0;

    /// @brief Draws the screen content.
    /// @param window The render window.
    virtual void draw(sf::RenderWindow& window) = 0;

    /// @brief Polls and routes all pending window events for this screen.
    /// @param window The render window.
    virtual void handleEvents(sf::RenderWindow& window);

    /// @brief Handles a single input event (key press, mouse click, etc.).
    /// @param event The event to handle.
    virtual void handleEvent(const sf::Event& /*event*/) {}

    /// @brief Handles a single input event with access to window coordinate mapping.
    /// @param event The event to handle.
    /// @param window The render window.
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& /*window*/) { handleEvent(event); }

    /// @brief Updates the screen logic.
    /// @param deltaTime Time elapsed since last frame.
    virtual void update(float /*deltaTime*/) {}

    /// @brief Returns true once the screen is done and wants to be replaced.
    virtual bool isFinished() const { return false; }

    /// @brief Reports the screen's outcome (defaults to Running for screens with no result).
    virtual LevelResult getResult() const { return LevelResult::Running; }

protected:
    /// @brief Shows an overlay owned by this screen.
    void showOverlay(std::unique_ptr<OverlayView> overlay);

    /// @brief Returns true while this screen has an active overlay.
    bool hasOverlay() const { return m_overlay != nullptr; }

    /// @brief Sends an event to the overlay when one is active.
    bool handleOverlayEvent(const sf::Event& event, const sf::RenderWindow& window);

    /// @brief Updates the overlay when one is active.
    bool updateOverlay(float deltaTime);

    /// @brief Draws the overlay above this screen's content.
    void drawOverlay(sf::RenderWindow& window) const;

    /// @brief Removes the overlay after it dismisses itself.
    void clearDismissedOverlay();

private:
    std::unique_ptr<OverlayView> m_overlay;
};
