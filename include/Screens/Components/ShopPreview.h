#pragma once

#include "App/State/GameState.h"
#include "Shop/ShopManager.h"
#include "Screens/Core/Screen.h"

#include <SFML/Graphics.hpp>
#include <map>

/// @brief Draws shop asset artwork and the modal shop preview overlay.
class ShopPreview {
public:
    /// @brief Creates a preview renderer around shared shop state and loaded resources.
    ShopPreview(GameState& gameState,
                const ShopManager& shopManager,
                const std::map<int, sf::Texture>& textures,
                const std::map<int, sf::Font>& fonts,
                const sf::Font& fallbackFont);

    /// @brief Draws texture, font, or music artwork for a shop asset.
    void drawAssetArtwork(sf::RenderTarget& target,
                          const ShopAsset& asset,
                          sf::FloatRect bounds,
                          bool owned,
                          bool large) const;

    /// @brief Draws the active preview modal if an asset is being previewed.
    void draw(sf::RenderTarget& target) const;

private:
    /// @brief Draws texture-backed asset artwork.
    void drawImageArtwork(sf::RenderTarget& target,
                          const ShopAsset& asset,
                          sf::FloatRect bounds,
                          bool owned) const;

    /// @brief Draws the shared background box for non-image artwork.
    void drawArtworkBox(sf::RenderTarget& target, sf::FloatRect bounds, bool owned) const;

    /// @brief Draws font preview artwork.
    void drawFontArtwork(sf::RenderTarget& target,
                         const ShopAsset& asset,
                         sf::FloatRect bounds,
                         bool large) const;

    /// @brief Shrinks a font sample until it fits the artwork box.
    static void fitFontSample(sf::Text& sample, sf::FloatRect bounds);

    /// @brief Draws music preview artwork.
    void drawMusicArtwork(sf::RenderTarget& target, sf::FloatRect bounds, bool large) const;

    /// @brief Draws the modal scrim and panel background.
    sf::FloatRect drawPreviewPanel(sf::RenderTarget& target) const;

    /// @brief Draws the preview title.
    void drawPreviewTitle(sf::RenderTarget& target, const ShopAsset& asset, sf::FloatRect panelBounds) const;

    /// @brief Returns the artwork bounds inside the modal.
    sf::FloatRect previewArtworkBounds(const ShopAsset& asset, sf::FloatRect panelBounds) const;

    /// @brief Draws ownership/price text inside the modal.
    void drawPreviewPrice(sf::RenderTarget& target, const ShopAsset& asset, sf::FloatRect panelBounds) const;

    /// @brief Draws the not-enough-coins warning when needed.
    void drawAffordabilityWarning(sf::RenderTarget& target,
                                  const ShopAsset& asset,
                                  sf::FloatRect panelBounds) const;

    /// @brief Draws all preview action buttons.
    void drawPreviewButtons(sf::RenderTarget& target, const ShopAsset& asset) const;

    /// @brief Draws one preview action button.
    void drawPreviewButton(sf::RenderTarget& target,
                           sf::FloatRect bounds,
                           const std::string& label,
                           bool enabled,
                           bool primary) const;

    /// @brief Returns the correct fill color for a preview button state.
    static sf::Color previewButtonColor(bool enabled, bool primary);

    /// @brief Returns the number of frames in a shop preview sprite sheet.
    static int shopPreviewFrameCount(ShopAssetType type);

    /// @brief Crops animated sprite sheets to their first preview frame.
    static void cropFirstShopFrame(sf::Sprite& sprite, ShopAssetType type);

    /// @brief Player state used to draw price and affordability state.
    GameState& m_gameState;

    /// @brief Shop interaction state used to discover the active preview asset and button state.
    const ShopManager& m_shopManager;

    /// @brief Preview textures owned by ShopScreen; referenced here to satisfy SFML sprite lifetimes.
    const std::map<int, sf::Texture>& m_textures;

    /// @brief Preview fonts owned by ShopScreen; referenced here to satisfy SFML text lifetimes.
    const std::map<int, sf::Font>& m_fonts;

    /// @brief Fallback UI font from ResourceManager.
    const sf::Font& m_fallbackFont;
};
