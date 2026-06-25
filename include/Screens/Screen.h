#pragma once
#include <SFML/Graphics.hpp>
#include "Enums/LevelResult.h"
#include "Screens/OverlayView.h"
#include <memory>

/// @brief App-level command requested by a screen.
///
/// A command is only a request, not permission to mutate the app. Game reads
/// this value, checks whether the active screen is allowed to request it, and
/// only then performs the window or screen transition.
enum class ScreenCommand {
    /// @brief No app-level action was requested.
    None,

    /// @brief Request to leave the start screen and create the gameplay screen.
    StartGame,

    /// @brief Request to leave gameplay and recreate the start screen.
    ReturnToStart,

    /// @brief Request to close the app; Game only honors this from StartScreen.
    Quit
};

/// @brief Event data derived from the window before dispatching to a screen.
struct ScreenEventContext {
    sf::Vector2f mousePosition{};
};

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

    /// @brief Updates the screen logic.
    /// @param deltaTime Time elapsed since last frame.
    virtual void update(float /*deltaTime*/) {}

    /// @brief Returns true once the screen is done and wants to be replaced.
    virtual bool isFinished() const { return false; }

    /// @brief Reports the screen's outcome (defaults to Running for screens with no result).
    virtual LevelResult getResult() const { return LevelResult::Running; }

    /// @brief Returns the command requested by this screen.
    ScreenCommand getCommand() const { return m_command; }

    /// @brief Clears the pending command after Game handles it.
    void clearCommand() { m_command = ScreenCommand::None; }

protected:
    /// @brief Handles one event after overlay routing.
    virtual void handleScreenEvent(const sf::Event& /*event*/, const ScreenEventContext& /*context*/) {}

    /// @brief Requests an app-level command for Game to execute.
    void requestCommand(ScreenCommand command) { m_command = command; }

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
