#pragma once

#include "Gameplay/GameState.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

/// @brief Owns shop interaction state and purchase/selection behavior.
class ShopManager {
public:
    /// @brief Action buttons available in the shop preview overlay.
    enum class PreviewAction {
        None,
        Primary,
        Try,
        Cancel
    };

    /// @brief Result of a shop click that may affect screen navigation.
    enum class ClickResult {
        None,
        ReturnToStart
    };

    /// @brief Creates shop logic for the current game state and catalog.
    /// @param gameState Mutable player state used for coins, ownership, and selections.
    /// @param catalog Shop assets loaded from game data.
    /// @param uncatalogedShopFiles Number of media files that are not listed in the catalog.
    ShopManager(GameState& gameState,
                const std::vector<ShopAsset>& catalog,
                int uncatalogedShopFiles);

    /// @brief Updates which shop tile is currently hovered.
    /// @param position Mouse position in screen coordinates.
    void updateHover(sf::Vector2f position);

    /// @brief Handles a left-click in shop coordinates.
    /// @param position Mouse position in screen coordinates.
    /// @return Navigation request produced by the click, if any.
    ClickResult handleClick(sf::Vector2f position);

    /// @brief Closes the active preview overlay and stops audio preview.
    void closePreview();

    /// @brief Returns the active shop category.
    ShopAssetType activeType() const { return m_activeType; }

    /// @brief Returns the asset currently under the mouse, or nullptr.
    const ShopAsset* hoveredAsset() const { return m_hoveredAsset; }

    /// @brief Returns the asset shown in the preview overlay, or nullptr.
    const ShopAsset* previewAsset() const { return m_previewAsset; }

    /// @brief Returns true when the shop has uncataloged media files.
    bool hasUncatalogedShopFiles() const { return m_uncatalogedShopFiles > 0; }

    /// @brief Returns shop assets in the currently active category.
    std::vector<const ShopAsset*> visibleAssets() const;

    /// @brief Returns true for background music assets.
    static bool isMusicAsset(const ShopAsset& asset);

    /// @brief Returns true for font assets.
    static bool isFontAsset(const ShopAsset& asset);

    /// @brief Returns true for texture-backed visual assets.
    static bool isImageAsset(const ShopAsset& asset);

    /// @brief Returns the primary preview button label for an asset.
    /// @param asset Asset shown in the preview overlay.
    /// @return "Selected", "Select", or "Buy" based on state.
    std::string primaryPreviewLabel(const ShopAsset& asset) const;

    /// @brief Returns whether the primary preview action can currently run.
    /// @param asset Asset shown in the preview overlay.
    /// @return True when the asset can be bought or selected.
    bool primaryPreviewEnabled(const ShopAsset& asset) const;

    /// @brief Returns the back-button bounds used by drawing and hit testing.
    static sf::FloatRect backButtonBounds();

    /// @brief Returns the tile bounds for a visible asset index.
    /// @param index Zero-based visible asset index.
    /// @return Rectangle occupied by the tile.
    static sf::FloatRect tileBounds(std::size_t index);

    /// @brief Returns the preview button bounds for the current preview.
    /// @param action Preview action to locate.
    /// @return Button bounds, or an empty rectangle when the action is unavailable.
    sf::FloatRect previewButtonBounds(PreviewAction action) const;

private:
    /// @brief Opens the preview overlay for an asset.
    /// @param asset Asset to preview.
    void openPreview(const ShopAsset& asset);

    /// @brief Runs a selected preview action.
    /// @param action Action chosen by the user.
    void activatePreviewAction(PreviewAction action);

    /// @brief Saves current user data through DataManager.
    void saveUserData() const;

    /// @brief Returns the visible asset under a screen position.
    /// @param position Mouse position in screen coordinates.
    /// @return Matching asset, or nullptr.
    const ShopAsset* tileAt(sf::Vector2f position) const;

    /// @brief Returns the category index under a screen position.
    /// @param position Mouse position in screen coordinates.
    /// @return Zero-based category index, or -1 when no category was hit.
    static int categoryIndexAt(sf::Vector2f position);

    /// @brief Returns the preview action button under a screen position.
    /// @param position Mouse position in screen coordinates.
    /// @return Matching preview action, or PreviewAction::None.
    PreviewAction previewActionAt(sf::Vector2f position) const;

    /// @brief Mutable player state used for coins, ownership, and selected assets.
    GameState& m_gameState;

    /// @brief Shop catalog owned by GameData; ShopManager only keeps references into it.
    const std::vector<ShopAsset>& m_catalog;

    /// @brief Number of shop media files present on disk but missing from gameData.json.
    int m_uncatalogedShopFiles = 0;

    /// @brief Category currently selected in the shop tabs.
    ShopAssetType m_activeType = ShopAssetType::CommandVehicle;

    /// @brief Asset currently under the mouse; points into m_catalog and is never owned here.
    const ShopAsset* m_hoveredAsset = nullptr;

    /// @brief Asset currently shown in the preview modal; points into m_catalog and is never owned here.
    const ShopAsset* m_previewAsset = nullptr;
};
