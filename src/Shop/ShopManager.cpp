#include "Shop/ShopManager.h"

#include "App/Constants.h"
#include "App/ScreenConstants.h"
#include "Resources/DataManager.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"
#include "Utilities/Utils.h"

#include <array>
#include <filesystem>
#include <set>

namespace {
const std::array<std::filesystem::path, 2> ShopAssetFolders = {
    "images/shop",
    "sounds/shop"
};
}

ShopManager::ShopManager(GameState& gameState,
                         const std::vector<ShopAsset>& catalog)
    : m_gameState(gameState)
    , m_catalog(catalog)
    , m_uncatalogedShopFiles(countUncatalogedShopFiles(catalog)) {
}

void ShopManager::updateHover(sf::Vector2f position) {
    m_hoveredAsset = m_previewAsset ? nullptr : tileAt(position);
}

ShopClickResult ShopManager::handleClick(sf::Vector2f position) {
    if (m_previewAsset) {
        activatePreviewAction(previewActionAt(position));
        return ShopClickResult::None;
    }

    if (GeometryUtils::contains(backButtonBounds(), position))
        return ShopClickResult::ReturnToStart;

    const int categoryIndex = categoryIndexAt(position);
    if (categoryIndex >= 0) {
        m_activeType = allShopAssetTypes()[static_cast<std::size_t>(categoryIndex)];
        m_hoveredAsset = nullptr;
        return ShopClickResult::None;
    }

    if (const ShopAsset* asset = tileAt(position))
        openPreview(*asset);

    return ShopClickResult::None;
}

void ShopManager::openPreview(const ShopAsset& asset) {
    m_previewAsset = &asset;
}

void ShopManager::closePreview() {
    SoundManager::instance().stopPreview();
    m_previewAsset = nullptr;
}

void ShopManager::activatePreviewAction(PreviewAction action) {
    if (!m_previewAsset || action == PreviewAction::None)
        return;

    if (action == PreviewAction::Cancel) {
        closePreview();
        return;
    }

    if (action == PreviewAction::Try) {
        if (isMusicAsset(*m_previewAsset))
            SoundManager::instance().playPreview(m_previewAsset->file);
        return;
    }

    if (!primaryPreviewEnabled(*m_previewAsset))
        return;

    if (m_gameState.ownsAsset(*m_previewAsset))
        m_gameState.selectAsset(*m_previewAsset);
    else
        m_gameState.buyAsset(*m_previewAsset);

    ResourceManager::instance().applyShopSelections(m_catalog, m_gameState.getUserData());
    m_gameState.setSelectedAssetExtras(m_catalog);
    saveUserData();
    closePreview();
}

void ShopManager::saveUserData() const {
    DataManager::instance().updateUserData(m_gameState.getUserData());
    DataManager::instance().saveUserData();
}

bool ShopManager::isMusicAsset(const ShopAsset& asset) {
    return asset.type == ShopAssetType::BackgroundMusic;
}

bool ShopManager::isFontAsset(const ShopAsset& asset) {
    return asset.type == ShopAssetType::Font;
}

bool ShopManager::isImageAsset(const ShopAsset& asset) {
    return !isMusicAsset(asset) && !isFontAsset(asset);
}

int ShopManager::countUncatalogedShopFiles(const std::vector<ShopAsset>& catalog) {
    std::set<std::string> catalogFiles;
    for (const ShopAsset& asset : catalog) {
        catalogFiles.insert(asset.file);
    }

    int count = 0;
    for (const std::filesystem::path& folder : ShopAssetFolders) {
        if (!std::filesystem::exists(folder)) {
            continue;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
            if (!entry.is_regular_file()) {
                continue;
            }
            if (isHiddenShopCompanionFile(entry.path())) {
                continue;
            }

            const std::string path = entry.path().generic_string();
            if (!catalogFiles.contains(path)) {
                ++count;
            }
        }
    }

    return count;
}

bool ShopManager::isHiddenShopCompanionFile(const std::filesystem::path& path) {
    return path.filename().generic_string().starts_with("start-background");
}

std::vector<const ShopAsset*> ShopManager::visibleAssets() const {
    std::vector<const ShopAsset*> assets;
    for (const ShopAsset& asset : m_catalog) {
        if (asset.type == m_activeType)
            assets.push_back(&asset);
    }

    return assets;
}

const ShopAsset* ShopManager::tileAt(sf::Vector2f position) const {
    const std::vector<const ShopAsset*> assets = visibleAssets();
    for (std::size_t index = 0; index < assets.size(); ++index) {
        if (GeometryUtils::contains(tileBounds(index), position))
            return assets[index];
    }

    return nullptr;
}

int ShopManager::categoryIndexAt(sf::Vector2f position) {
    const std::vector<ShopAssetType> types = allShopAssetTypes();
    for (std::size_t index = 0; index < types.size(); ++index) {
        const float x = ShopScreenConfig::SidePadding +
            static_cast<float>(index) * (ShopScreenConfig::CategoryWidth + ShopScreenConfig::CategoryGap);
        const sf::FloatRect bounds(
            { x, ShopScreenConfig::CategoryTop },
            { ShopScreenConfig::CategoryWidth, ShopScreenConfig::CategoryHeight });
        if (GeometryUtils::contains(bounds, position))
            return static_cast<int>(index);
    }

    return -1;
}

PreviewAction ShopManager::previewActionAt(sf::Vector2f position) const {
    const std::array<PreviewAction, 3> actions = {
        PreviewAction::Primary,
        PreviewAction::Try,
        PreviewAction::Cancel
    };

    for (PreviewAction action : actions) {
        if (GeometryUtils::contains(previewButtonBounds(action), position))
            return action;
    }

    return PreviewAction::None;
}

std::string ShopManager::primaryPreviewLabel(const ShopAsset& asset) const {
    if (m_gameState.isAssetSelected(asset))
        return "Selected";
    if (m_gameState.ownsAsset(asset))
        return "Select";
    return "Buy";
}

bool ShopManager::primaryPreviewEnabled(const ShopAsset& asset) const {
    if (m_gameState.isAssetSelected(asset))
        return false;
    if (m_gameState.ownsAsset(asset))
        return true;
    return m_gameState.canAfford(asset);
}

sf::FloatRect ShopManager::backButtonBounds() {
    return {
        { ShopScreenConfig::SidePadding, ShopScreenConfig::TopPadding },
        { ShopScreenConfig::BackButtonWidth, ShopScreenConfig::BackButtonHeight }
    };
}

sf::FloatRect ShopManager::tileBounds(std::size_t index) {
    const std::size_t columns = 5;
    const std::size_t row = index / columns;
    const std::size_t column = index % columns;
    return {
        { ShopScreenConfig::SidePadding +
              static_cast<float>(column) * (ShopScreenConfig::TileWidth + ShopScreenConfig::TileGap),
          ShopScreenConfig::GridTop +
              static_cast<float>(row) * (ShopScreenConfig::TileHeight + ShopScreenConfig::TileGap) },
        { ShopScreenConfig::TileWidth, ShopScreenConfig::TileHeight }
    };
}

sf::FloatRect ShopManager::previewButtonBounds(PreviewAction action) const {
    if (!m_previewAsset)
        return { { 0.f, 0.f }, { 0.f, 0.f } };

    const bool music = isMusicAsset(*m_previewAsset);
    const float width = OverlayViewConfig::ButtonWidth;
    const float height = OverlayViewConfig::ButtonHeight;
    const float gap = OverlayViewConfig::ButtonGap;
    const float y = WindowConfig::Height / 2.f + 205.f;
    const int buttonCount = music ? 3 : 2;
    const float totalWidth = static_cast<float>(buttonCount) * width +
        static_cast<float>(buttonCount - 1) * gap;
    const float startX = WindowConfig::Width / 2.f - totalWidth / 2.f;

    int index = 0;
    if (music) {
        if (action == PreviewAction::Try)
            index = 1;
        else if (action == PreviewAction::Cancel)
            index = 2;
    } else if (action == PreviewAction::Cancel) {
        index = 1;
    } else if (action == PreviewAction::Try) {
        return { { 0.f, 0.f }, { 0.f, 0.f } };
    }

    return {
        { startX + static_cast<float>(index) * (width + gap), y },
        { width, height }
    };
}
