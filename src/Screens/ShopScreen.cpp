#include "Screens/ShopScreen.h"

#include "Gameplay/Constants.h"
#include "Gameplay/DataManager.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <string>

namespace {
const std::string UserDataPath = "userData.json";

bool contains(const sf::FloatRect& bounds, sf::Vector2f position) {
    return position.x >= bounds.position.x &&
        position.x <= bounds.position.x + bounds.size.x &&
        position.y >= bounds.position.y &&
        position.y <= bounds.position.y + bounds.size.y;
}

sf::Vector2f centerOf(const sf::FloatRect& bounds) {
    return {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
}

void fitSpriteTo(sf::Sprite& sprite, sf::FloatRect bounds) {
    const sf::IntRect textureRect = sprite.getTextureRect();
    const sf::Vector2i size = textureRect.size;
    const float scale = std::min(
        bounds.size.x / static_cast<float>(size.x),
        bounds.size.y / static_cast<float>(size.y));
    sprite.setScale({ scale, scale });
    GraphicsUtils::centerOrigin(sprite);
    sprite.setPosition(centerOf(bounds));
}

int shopPreviewFrameCount(ShopAssetType type) {
    switch (type) {
        case ShopAssetType::CommandVehicle:
            return VehicleConfig::FrameCount;
        case ShopAssetType::Drone:
            return DroneConfig::FrameCount;
        case ShopAssetType::Background:
        case ShopAssetType::BackgroundMusic:
        case ShopAssetType::Font:
            return 1;
    }

    return 1;
}

void cropFirstShopFrame(sf::Sprite& sprite, ShopAssetType type) {
    const int frameCount = shopPreviewFrameCount(type);
    if (frameCount <= 1)
        return;

    const sf::Vector2u size = sprite.getTexture().getSize();
    sprite.setTextureRect(sf::IntRect(
        { 0, 0 },
        { static_cast<int>(size.x / static_cast<unsigned int>(frameCount)),
          static_cast<int>(size.y) }));
}
}

ShopScreen::ShopScreen(GameState& gameState,
                       const std::vector<ShopAsset>& catalog,
                       int uncatalogedShopFiles)
    : m_gameState(gameState)
    , m_catalog(catalog)
    , m_uncatalogedShopFiles(uncatalogedShopFiles)
    , m_background({ WindowConfig::Width, WindowConfig::Height })
    , m_font(ResourceManager::instance().getFont(Fonts::StencilForce)) {
    loadTextures();
    configureLayout();
}

void ShopScreen::loadTextures() {
    for (const ShopAsset& asset : m_catalog) {
        if (isImageAsset(asset))
            m_textures.emplace(asset.id, ResourceManager::instance().loadTextureFromFile(asset.file));
        else if (isFontAsset(asset))
            m_fonts.emplace(asset.id, ResourceManager::instance().loadFontFromFile(asset.file));
    }
}

void ShopScreen::configureLayout() {
    m_background.setFillColor(ShopScreenConfig::BackgroundColor);
}

void ShopScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (event.is<sf::Event::MouseMoved>())
        updateHover(context.mousePosition);

    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left)
            handleClick(context.mousePosition);
    }

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Escape) {
            if (m_previewAsset)
                closePreview();
            else
                requestCommand(ScreenCommand::ReturnToStart);
        }
    }
}

void ShopScreen::update(float /*deltaTime*/) {
}

void ShopScreen::draw(sf::RenderWindow& target) {
    target.draw(m_background);
    drawHeader(target);
    drawCategories(target);
    drawTiles(target);
    drawPreview(target);
}

void ShopScreen::updateHover(sf::Vector2f position) {
    m_hoveredAsset = m_previewAsset ? nullptr : tileAt(position);
}

void ShopScreen::handleClick(sf::Vector2f position) {
    if (m_previewAsset) {
        activatePreviewAction(previewActionAt(position));
        return;
    }

    if (contains(backButtonBounds(), position)) {
        requestCommand(ScreenCommand::ReturnToStart);
        return;
    }

    const int categoryIndex = categoryIndexAt(position);
    if (categoryIndex >= 0) {
        m_activeType = allShopAssetTypes()[static_cast<std::size_t>(categoryIndex)];
        m_hoveredAsset = nullptr;
        return;
    }

    if (const ShopAsset* asset = tileAt(position))
        openPreview(*asset);
}

void ShopScreen::openPreview(const ShopAsset& asset) {
    m_previewAsset = &asset;
}

void ShopScreen::closePreview() {
    SoundManager::instance().stopPreview();
    m_previewAsset = nullptr;
}

void ShopScreen::activatePreviewAction(PreviewAction action) {
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
    saveUserData();
    closePreview();
}

void ShopScreen::saveUserData() const {
    DataManager::saveUserData(UserDataPath, m_gameState.getUserData());
}

bool ShopScreen::isMusicAsset(const ShopAsset& asset) const {
    return asset.type == ShopAssetType::BackgroundMusic;
}

bool ShopScreen::isFontAsset(const ShopAsset& asset) const {
    return asset.type == ShopAssetType::Font;
}

bool ShopScreen::isImageAsset(const ShopAsset& asset) const {
    return !isMusicAsset(asset) && !isFontAsset(asset);
}

std::vector<const ShopAsset*> ShopScreen::visibleAssets() const {
    std::vector<const ShopAsset*> assets;
    for (const ShopAsset& asset : m_catalog) {
        if (asset.type == m_activeType)
            assets.push_back(&asset);
    }

    return assets;
}

const ShopAsset* ShopScreen::tileAt(sf::Vector2f position) const {
    const std::vector<const ShopAsset*> assets = visibleAssets();
    for (std::size_t index = 0; index < assets.size(); ++index) {
        if (contains(tileBounds(index), position))
            return assets[index];
    }

    return nullptr;
}

int ShopScreen::categoryIndexAt(sf::Vector2f position) const {
    const std::vector<ShopAssetType> types = allShopAssetTypes();
    for (std::size_t index = 0; index < types.size(); ++index) {
        const float x = ShopScreenConfig::SidePadding +
            static_cast<float>(index) * (ShopScreenConfig::CategoryWidth + ShopScreenConfig::CategoryGap);
        const sf::FloatRect bounds(
            { x, ShopScreenConfig::CategoryTop },
            { ShopScreenConfig::CategoryWidth, ShopScreenConfig::CategoryHeight });
        if (contains(bounds, position))
            return static_cast<int>(index);
    }

    return -1;
}

ShopScreen::PreviewAction ShopScreen::previewActionAt(sf::Vector2f position) const {
    const std::array<PreviewAction, 3> actions = {
        PreviewAction::Primary,
        PreviewAction::Try,
        PreviewAction::Cancel
    };

    for (PreviewAction action : actions) {
        if (contains(previewButtonBounds(action), position))
            return action;
    }

    return PreviewAction::None;
}

std::string ShopScreen::primaryPreviewLabel(const ShopAsset& asset) const {
    if (m_gameState.isAssetSelected(asset))
        return "Selected";
    if (m_gameState.ownsAsset(asset))
        return "Select";
    return "Buy";
}

bool ShopScreen::primaryPreviewEnabled(const ShopAsset& asset) const {
    if (m_gameState.isAssetSelected(asset))
        return false;
    if (m_gameState.ownsAsset(asset))
        return true;
    return m_gameState.canAfford(asset);
}

void ShopScreen::drawHeader(sf::RenderTarget& target) const {
    sf::RectangleShape back(backButtonBounds().size);
    back.setPosition(backButtonBounds().position);
    back.setFillColor(ShopScreenConfig::PanelColor);
    back.setOutlineColor(ShopScreenConfig::OutlineColor);
    back.setOutlineThickness(2.f);
    target.draw(back);

    sf::Text backText = makeText("Back", ShopScreenConfig::ButtonTextSize,
        centerOf(backButtonBounds()), ShopScreenConfig::TextColor);
    GraphicsUtils::centerOrigin(backText);
    target.draw(backText);

    sf::Text title = makeText("Shop", ShopScreenConfig::TitleTextSize,
        { WindowConfig::Width / 2.f, ShopScreenConfig::TopPadding + 20.f },
        ShopScreenConfig::TextColor);
    GraphicsUtils::centerOrigin(title);
    target.draw(title);

    sf::Text coins = makeText("Coins: " + std::to_string(m_gameState.getCoins()),
        ShopScreenConfig::ButtonTextSize,
        { WindowConfig::Width - ShopScreenConfig::SidePadding - 280.f, ShopScreenConfig::TopPadding + 15.f },
        ShopScreenConfig::TextColor);
    target.draw(coins);

    if (m_uncatalogedShopFiles > 0) {
        sf::CircleShape warning(ShopScreenConfig::WarningDotRadius);
        warning.setFillColor(ShopScreenConfig::WarningColor);
        warning.setPosition({
            WindowConfig::Width - ShopScreenConfig::SidePadding - ShopScreenConfig::WarningDotRadius * 2.f,
            ShopScreenConfig::TopPadding
        });
        target.draw(warning);
    }
}

void ShopScreen::drawCategories(sf::RenderTarget& target) const {
    const std::vector<ShopAssetType> types = allShopAssetTypes();
    for (std::size_t index = 0; index < types.size(); ++index) {
        const ShopAssetType type = types[index];
        const float x = ShopScreenConfig::SidePadding +
            static_cast<float>(index) * (ShopScreenConfig::CategoryWidth + ShopScreenConfig::CategoryGap);
        const sf::FloatRect bounds(
            { x, ShopScreenConfig::CategoryTop },
            { ShopScreenConfig::CategoryWidth, ShopScreenConfig::CategoryHeight });

        sf::RectangleShape tab(bounds.size);
        tab.setPosition(bounds.position);
        tab.setFillColor(type == m_activeType ? ShopScreenConfig::SelectedColor : ShopScreenConfig::PanelColor);
        tab.setOutlineColor(ShopScreenConfig::OutlineColor);
        tab.setOutlineThickness(2.f);
        target.draw(tab);

        sf::Text text = makeText(shopAssetTypeToTitle(type), ShopScreenConfig::ButtonTextSize,
            centerOf(bounds), ShopScreenConfig::TextColor);
        GraphicsUtils::centerOrigin(text);
        target.draw(text);
    }
}

void ShopScreen::drawTiles(sf::RenderTarget& target) const {
    const std::vector<const ShopAsset*> assets = visibleAssets();
    for (std::size_t index = 0; index < assets.size(); ++index)
        drawTile(target, *assets[index], tileBounds(index), assets[index] == m_hoveredAsset);
}

void ShopScreen::drawTile(sf::RenderTarget& target,
                          const ShopAsset& asset,
                          sf::FloatRect bounds,
                          bool hovered) const {
    const bool owned = m_gameState.ownsAsset(asset);
    const bool selected = m_gameState.isAssetSelected(asset);

    sf::RectangleShape tile(bounds.size);
    tile.setPosition(bounds.position);
    tile.setFillColor(selected ? ShopScreenConfig::SelectedColor :
        (hovered ? ShopScreenConfig::PanelHoverColor : ShopScreenConfig::PanelColor));
    tile.setOutlineColor(ShopScreenConfig::OutlineColor);
    tile.setOutlineThickness(selected ? 4.f : 2.f);
    target.draw(tile);

    const sf::FloatRect imageBounds(
        { bounds.position.x + 8.f, bounds.position.y + 12.f },
        { bounds.size.x - 16.f, 244.f });
    drawAssetArtwork(target, asset, imageBounds, owned, false);

    if (!owned) {
        sf::Sprite lock(ResourceManager::instance().getImage(GameObjects::Lock));
        const sf::FloatRect lockBounds(
            { bounds.position.x + bounds.size.x / 2.f - ShopScreenConfig::LockSize / 2.f,
              bounds.position.y + 95.f },
            { ShopScreenConfig::LockSize, ShopScreenConfig::LockSize });
        fitSpriteTo(lock, lockBounds);
        target.draw(lock);
    }

    drawWrappedTileName(target,
        asset.name,
        { bounds.position.x + 22.f, bounds.position.y + 266.f },
        bounds.size.x - 44.f);

    std::string price = selected ? "Selected" : (owned ? "Owned" : std::to_string(asset.price) + " coins");
    drawFittedText(target,
        price,
        ShopScreenConfig::TilePriceSize,
        14,
        { bounds.position.x + 22.f, bounds.position.y + 326.f },
        bounds.size.x - 44.f,
        selected ? ShopScreenConfig::TextColor : ShopScreenConfig::MutedTextColor);
}

void ShopScreen::drawAssetArtwork(sf::RenderTarget& target,
                                  const ShopAsset& asset,
                                  sf::FloatRect bounds,
                                  bool owned,
                                  bool large) const {
    if (isImageAsset(asset)) {
        const auto texture = m_textures.find(asset.id);
        if (texture == m_textures.end())
            return;

        sf::Sprite sprite(texture->second);
        cropFirstShopFrame(sprite, asset.type);

        fitSpriteTo(sprite, bounds);
        if (!owned)
            sprite.setColor(ShopScreenConfig::LockedColor);
        target.draw(sprite);
        return;
    }

    sf::RectangleShape box(bounds.size);
    box.setPosition(bounds.position);
    box.setFillColor(owned ? sf::Color(30, 45, 55, 230) : sf::Color(30, 45, 55, 115));
    box.setOutlineColor(ShopScreenConfig::OutlineColor);
    box.setOutlineThickness(2.f);
    target.draw(box);

    if (isFontAsset(asset)) {
        const auto font = m_fonts.find(asset.id);
        const sf::Font& previewFont = font == m_fonts.end() ? m_font : font->second;
        sf::Text sample(previewFont, large ? "Aa 123" : "Aa", large ? 46u : 54u);
        sample.setFillColor(ShopScreenConfig::TextColor);
        while ((sample.getLocalBounds().size.x > bounds.size.x - 24.f ||
                sample.getLocalBounds().size.y > bounds.size.y - 24.f) &&
               sample.getCharacterSize() > 24u) {
            sample.setCharacterSize(sample.getCharacterSize() - 2u);
        }
        GraphicsUtils::centerOrigin(sample);
        sample.setPosition(centerOf(bounds));
        target.draw(sample);
        return;
    }

    sf::Text music = makeText(large ? "Music Preview" : "Music",
        large ? OverlayViewConfig::BodyTextSize : ShopScreenConfig::ButtonTextSize,
        centerOf(bounds),
        ShopScreenConfig::TextColor);
    GraphicsUtils::centerOrigin(music);
    target.draw(music);
}

void ShopScreen::drawWrappedTileName(sf::RenderTarget& target,
                                     const std::string& name,
                                     sf::Vector2f position,
                                     float maxWidth) const {
    const std::vector<std::string> lines = wrapTextLines(
        name,
        ShopScreenConfig::TileNameSize,
        maxWidth,
        2);

    for (std::size_t index = 0; index < lines.size(); ++index) {
        sf::Text line = makeText(lines[index],
            ShopScreenConfig::TileNameSize,
            { position.x, position.y + static_cast<float>(index) * 29.f },
            ShopScreenConfig::TextColor);
        while (line.getLocalBounds().size.x > maxWidth && line.getCharacterSize() > 16u)
            line.setCharacterSize(line.getCharacterSize() - 1u);
        target.draw(line);
    }
}

void ShopScreen::drawFittedText(sf::RenderTarget& target,
                                const std::string& value,
                                unsigned int size,
                                unsigned int minSize,
                                sf::Vector2f position,
                                float maxWidth,
                                sf::Color color) const {
    sf::Text text = makeText(value, size, position, color);
    while (text.getLocalBounds().size.x > maxWidth && text.getCharacterSize() > minSize)
        text.setCharacterSize(text.getCharacterSize() - 1u);
    target.draw(text);
}

std::vector<std::string> ShopScreen::wrapTextLines(const std::string& text,
                                                   unsigned int size,
                                                   float maxWidth,
                                                   std::size_t maxLines) const {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string word;
    std::string current;

    while (stream >> word) {
        const std::string candidate = current.empty() ? word : current + " " + word;
        sf::Text candidateText(m_font, candidate, size);
        if (candidateText.getLocalBounds().size.x <= maxWidth || current.empty()) {
            current = candidate;
            continue;
        }

        lines.push_back(current);
        current = word;

        if (lines.size() + 1 >= maxLines)
            break;
    }

    if (!current.empty())
        lines.push_back(current);

    while (stream >> word) {
        if (lines.empty())
            lines.push_back(word);
        else
            lines.back() += " " + word;
    }

    if (lines.size() > maxLines)
        lines.resize(maxLines);

    if (!lines.empty()) {
        sf::Text lastLine(m_font, lines.back(), size);
        while (lastLine.getLocalBounds().size.x > maxWidth && lines.back().size() > 3) {
            lines.back().erase(lines.back().size() - 4);
            lines.back() += "...";
            lastLine.setString(lines.back());
        }
    }

    return lines;
}

void ShopScreen::drawPreview(sf::RenderTarget& target) const {
    if (!m_previewAsset)
        return;

    sf::RectangleShape scrim({ WindowConfig::Width, WindowConfig::Height });
    scrim.setFillColor(OverlayViewConfig::ScrimColor);
    target.draw(scrim);

    const sf::FloatRect panelBounds(
        { WindowConfig::Width / 2.f - 430.f, WindowConfig::Height / 2.f - 290.f },
        { 860.f, 580.f });
    sf::RectangleShape panel(panelBounds.size);
    panel.setPosition(panelBounds.position);
    panel.setFillColor(OverlayViewConfig::PanelColor);
    panel.setOutlineColor(OverlayViewConfig::PanelOutlineColor);
    panel.setOutlineThickness(OverlayViewConfig::PanelOutlineThickness);
    target.draw(panel);

    sf::Text title = makeText(m_previewAsset->name, OverlayViewConfig::TitleTextSize,
        { WindowConfig::Width / 2.f, panelBounds.position.y + 56.f }, OverlayViewConfig::TitleColor);
    GraphicsUtils::centerOrigin(title);
    target.draw(title);

    const bool imagePreview = isImageAsset(*m_previewAsset);
    const sf::FloatRect previewBounds(
        imagePreview
            ? sf::Vector2f{ panelBounds.position.x + 38.f, panelBounds.position.y + 105.f }
            : sf::Vector2f{ WindowConfig::Width / 2.f - 190.f, panelBounds.position.y + 110.f },
        imagePreview
            ? sf::Vector2f{ panelBounds.size.x - 76.f, 285.f }
            : sf::Vector2f{ 380.f, 260.f });

    drawAssetArtwork(target, *m_previewAsset, previewBounds, true, true);

    const std::string price = m_gameState.ownsAsset(*m_previewAsset)
        ? "Owned"
        : "Price: " + std::to_string(m_previewAsset->price) + " coins";
    sf::Text priceText = makeText(price, OverlayViewConfig::BodyTextSize,
        { WindowConfig::Width / 2.f, panelBounds.position.y + 405.f },
        ShopScreenConfig::TextColor);
    GraphicsUtils::centerOrigin(priceText);
    target.draw(priceText);

    if (!m_gameState.ownsAsset(*m_previewAsset) && !m_gameState.canAfford(*m_previewAsset)) {
        sf::Text warning = makeText("Not enough coins", 24,
            { WindowConfig::Width / 2.f, panelBounds.position.y + 442.f },
            ShopScreenConfig::WarningColor);
        GraphicsUtils::centerOrigin(warning);
        target.draw(warning);
    }

    drawPreviewButton(target, previewButtonBounds(PreviewAction::Primary),
        primaryPreviewLabel(*m_previewAsset), primaryPreviewEnabled(*m_previewAsset), true);

    if (isMusicAsset(*m_previewAsset)) {
        drawPreviewButton(target, previewButtonBounds(PreviewAction::Try), "Try", true, false);
    }

    drawPreviewButton(target, previewButtonBounds(PreviewAction::Cancel), "Cancel", true, false);
}

void ShopScreen::drawPreviewButton(sf::RenderTarget& target,
                                   sf::FloatRect bounds,
                                   const std::string& label,
                                   bool enabled,
                                   bool primary) const {
    sf::RectangleShape button(bounds.size);
    button.setPosition(bounds.position);
    button.setFillColor(enabled
        ? (primary ? OverlayViewConfig::PrimaryButtonColor : OverlayViewConfig::DefaultButtonColor)
        : OverlayViewConfig::DisabledButtonColor);
    button.setOutlineColor(OverlayViewConfig::ButtonOutlineColor);
    button.setOutlineThickness(OverlayViewConfig::ButtonOutlineThickness);
    target.draw(button);

    sf::Text text = makeText(label, OverlayViewConfig::ButtonTextSize,
        centerOf(bounds), OverlayViewConfig::ButtonTextColor);
    GraphicsUtils::centerOrigin(text);
    target.draw(text);
}

sf::Text ShopScreen::makeText(const std::string& value,
                              unsigned int size,
                              sf::Vector2f position,
                              sf::Color color) const {
    sf::Text text(m_font, value, size);
    text.setFillColor(color);
    text.setPosition(position);
    return text;
}

sf::FloatRect ShopScreen::backButtonBounds() const {
    return {
        { ShopScreenConfig::SidePadding, ShopScreenConfig::TopPadding },
        { ShopScreenConfig::BackButtonWidth, ShopScreenConfig::BackButtonHeight }
    };
}

sf::FloatRect ShopScreen::tileBounds(std::size_t index) const {
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

sf::FloatRect ShopScreen::previewButtonBounds(PreviewAction action) const {
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
