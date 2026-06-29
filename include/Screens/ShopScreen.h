#pragma once

#include "Shop/ShopManager.h"
#include "Screens/Core/Screen.h"
#include "Screens/Components/ShopPreview.h"

#include <SFML/Graphics.hpp>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class ShopScreen : public Screen {
public:
#pragma region Public
    /// @brief Creates the visual shop screen around a ShopManager.
    ShopScreen(GameState& gameState, const std::vector<ShopAsset>& catalog);

    /// @brief Handles shop input and delegates shop actions to ShopManager.
    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;

    /// @brief Updates shop screen animations.
    void update(float deltaTime) override;

    /// @brief Draws shop chrome, asset tiles, and the preview overlay.
    void draw(sf::RenderWindow& target) override;
#pragma endregion

private:
#pragma region Private
    /// @brief Loads textures and fonts needed for shop previews.
    void loadTextures();

    /// @brief Configures reusable SFML shapes used by the screen.
    void configureLayout();

    /// @brief Draws the back button, title, coins, and catalog warning indicator.
    void drawHeader(sf::RenderTarget& target) const;

    /// @brief Draws the back button in the shop header.
    void drawBackButton(sf::RenderTarget& target) const;

    /// @brief Draws title and coin count in the shop header.
    void drawHeaderText(sf::RenderTarget& target) const;

    /// @brief Draws the uncataloged-files warning dot when needed.
    void drawUncatalogedWarning(sf::RenderTarget& target) const;

    /// @brief Draws shop category tabs.
    void drawCategories(sf::RenderTarget& target) const;

    /// @brief Draws one shop category tab.
    void drawCategoryTab(sf::RenderTarget& target, ShopAssetType type, sf::FloatRect bounds) const;

    /// @brief Draws the visible asset tiles for the active category.
    void drawTiles(sf::RenderTarget& target) const;

    /// @brief Draws one shop asset tile.
    void drawTile(sf::RenderTarget& target,
                  const ShopAsset& asset,
                  sf::FloatRect bounds,
                  bool hovered) const;

    /// @brief Draws the tile panel background.
    void drawTilePanel(sf::RenderTarget& target,
                       sf::FloatRect bounds,
                       bool selected,
                       bool hovered) const;

    /// @brief Draws the lock icon over a locked tile.
    void drawLockIcon(sf::RenderTarget& target, sf::FloatRect tileBounds) const;

    /// @brief Draws selected, owned, or price text on a tile.
    void drawTilePrice(sf::RenderTarget& target,
                       const ShopAsset& asset,
                       sf::FloatRect bounds,
                       bool owned,
                       bool selected) const;

    /// @brief Draws a tile title wrapped to fit its tile.
    void drawWrappedTileName(sf::RenderTarget& target,
                             const std::string& name,
                             sf::Vector2f position,
                             float maxWidth) const;

    /// @brief Draws text while shrinking it until it fits the requested width.
    void drawFittedText(sf::RenderTarget& target,
                        const std::string& value,
                        unsigned int size,
                        unsigned int minSize,
                        sf::Vector2f position,
                        float maxWidth,
                        sf::Color color) const;

    /// @brief Splits a text string into lines that fit a maximum width.
    std::vector<std::string> wrapTextLines(const std::string& text,
                                           unsigned int size,
                                           float maxWidth,
                                           std::size_t maxLines) const;

    /// @brief Adds words to wrapped lines until the max line count is reached.
    void collectWrappedLines(std::istringstream& stream,
                             std::vector<std::string>& lines,
                             unsigned int size,
                             float maxWidth,
                             std::size_t maxLines) const;

    /// @brief Appends any remaining words to the last wrapped line.
    static void appendRemainingWords(std::istringstream& stream, std::vector<std::string>& lines);

    /// @brief Trims the last line with an ellipsis until it fits.
    void trimLastLine(std::vector<std::string>& lines, unsigned int size, float maxWidth) const;

    /// @brief Player state reference used only for drawing current coins and ownership labels.
    GameState& m_gameState;

    /// @brief Shop catalog reference used to load preview resources.
    const std::vector<ShopAsset>& m_catalog;

    /// @brief Interaction and purchase logic separated from SFML drawing.
    ShopManager m_shopManager;

    /// @brief Preview textures cached here so SFML sprites never reference temporary textures.
    mutable std::map<int, sf::Texture> m_textures;

    /// @brief Preview fonts cached here so SFML text never references temporary fonts.
    mutable std::map<int, sf::Font> m_fonts;

    /// @brief Reused full-screen background rectangle.
    sf::RectangleShape m_background;

    /// @brief Main shop UI font owned by ResourceManager and reused by all text drawing.
    const sf::Font& m_font;

    /// @brief Preview overlay and asset-artwork renderer kept out of ShopScreen.
    ShopPreview m_preview;
#pragma endregion
};
