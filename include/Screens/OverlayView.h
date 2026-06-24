#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

struct ScreenEventContext;

/// @brief Visual style for an overlay action button.
enum class OverlayActionType {
    Default,
    Primary,
    Danger
};

/// @brief A labeled overlay button with optional styling and behavior.
struct OverlayAction {
    std::string label;

    OverlayActionType type = OverlayActionType::Default;

    std::function<void()> action;
};

/// @brief Reusable modal-style overlay with text and action buttons.
class OverlayView : public sf::Drawable, public sf::Transformable {
public:
    /// @brief Creates an overlay with title text, optional body lines, and action buttons.
    /// @param title Main title shown at the top of the overlay.
    /// @param bodyLines Text lines shown below the title.
    /// @param actions Buttons shown at the bottom of the overlay.
    OverlayView(std::string title,
                std::vector<std::string> bodyLines,
                std::vector<OverlayAction> actions);

    /// @brief Handles keyboard and mouse events for selection and activation.
    /// @param event The SFML event to process.
    /// @param context Event data derived by Game.
    void handleEvent(const sf::Event& event, const ScreenEventContext& context);

    /// @brief Updates overlay animation or time-based state.
    /// @param dt Time elapsed since the previous frame.
    void update(float dt);

    /// @brief Returns true after an action has dismissed the overlay.
    bool isDismissed() const { return m_dismissed; }

protected:
    /// @brief Draws the overlay background, text, and buttons.
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    /// @brief Verifies that the overlay has a supported number of actions.
    void validateActions() const;

    /// @brief Computes panel and button geometry.
    void configureLayout();

    /// @brief Selects the button currently under the mouse cursor.
    void updateSelectionFromMouse(sf::Vector2f mousePosition);

    /// @brief Returns the button index at a position, or -1 if none.
    int buttonIndexAt(sf::Vector2f position) const;

    /// @brief Dismisses the overlay and runs the selected action.
    void activateAction(std::size_t actionIndex);

    /// @brief Moves keyboard selection to the previous button.
    void moveSelectionLeft();

    /// @brief Moves keyboard selection to the next button.
    void moveSelectionRight();

    /// @brief Draws the overlay title and body text.
    void drawText(sf::RenderTarget& target, sf::RenderStates states) const;

    /// @brief Draws all action buttons.
    void drawButtons(sf::RenderTarget& target, sf::RenderStates states) const;

    /// @brief Resolves a button color from its action type and selection state.
    sf::Color buttonColor(const OverlayAction& action, bool selected) const;

    /// @brief Creates centered text using the overlay font.
    sf::Text makeCenteredText(const std::string& value,
                              unsigned int size,
                              sf::Vector2f position,
                              sf::Color color) const;

    std::string m_title;
    std::vector<std::string> m_bodyLines;
    std::vector<OverlayAction> m_actions;
    std::vector<sf::FloatRect> m_buttonBounds;

    sf::RectangleShape m_scrim;
    sf::RectangleShape m_panel;

    const sf::Font& m_font;
    std::size_t m_selectedAction = 0;
    bool m_dismissed = false;
};
