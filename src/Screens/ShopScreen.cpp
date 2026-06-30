#include "Screens/ShopScreen.h"

#include "App/ButtonStyles.h"
#include "App/Constants.h"
#include "App/ScreenConstants.h"
#include "Resources/ResourceManager.h"
#include "Screens/Components/TextButton.h"
#include "Utilities/Utils.h"

#include <string>

ShopScreen::ShopScreen(GameState& gameState, const std::vector<ShopAsset>& catalog)
    : m_gameState(gameState)
    , m_catalog(catalog)
    , m_shopManager(gameState, catalog)
    , m_background({ WindowConfig::Width, WindowConfig::Height })
    , m_font(ResourceManager::instance().getFont(Fonts::StencilForce))
    , m_preview(gameState, m_shopManager, m_textures, m_fonts, m_font) {
    loadTextures();
    configureLayout();
}

#pragma region Resource Loading
void ShopScreen::loadTextures() {
    for (const ShopAsset& asset : m_catalog) {
        if (ShopManager::isImageAsset(asset))
            m_textures.emplace(asset.id, ResourceManager::instance().loadTextureFromFile(asset.file));
        else if (ShopManager::isFontAsset(asset))
            m_fonts.emplace(asset.id, ResourceManager::instance().loadFontFromFile(asset.file));
    }
}

void ShopScreen::configureLayout() {
    m_background.setFillColor(ShopScreenConfig::BackgroundColor);
}
#pragma endregion

#pragma region Event Handling
void ShopScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (isMouseMoved(event))
        m_shopManager.updateHover(context.mousePosition);

    if (isLeftMousePressed(event)) {
        if (m_shopManager.handleClick(context.mousePosition) == ShopClickResult::ReturnToStart)
            requestCommand(ScreenCommand::ReturnToStart);
    }

    if (isKeyPressed(event, sf::Keyboard::Key::Escape)) {
        if (m_shopManager.previewAsset())
            m_shopManager.closePreview();
        else
            requestCommand(ScreenCommand::ReturnToStart);
    }
}

void ShopScreen::update(float /*deltaTime*/) {
}
#pragma endregion

#pragma region Drawing
void ShopScreen::draw(sf::RenderWindow& target) {
    target.draw(m_background);
    drawHeader(target);
    drawCategories(target);
    drawTiles(target);
    m_preview.draw(target);
}

void ShopScreen::drawHeader(sf::RenderTarget& target) const {
    drawBackButton(target);
    drawHeaderText(target);
    drawUncatalogedWarning(target);
}

void ShopScreen::drawBackButton(sf::RenderTarget& target) const {
    TextButton button(m_font, "Back", ShopManager::backButtonBounds());
    button.setColors(ButtonStyles::shopColors(ShopScreenConfig::PanelColor));
    button.setMetrics(ButtonStyles::shopMetrics(ShopScreenConfig::ButtonTextSize, 18));
    target.draw(button);
}

void ShopScreen::drawHeaderText(sf::RenderTarget& target) const {
    sf::Text title = makeCenteredText(m_font, "Shop", ShopScreenConfig::TitleTextSize,
        { WindowConfig::Width / 2.f, ShopScreenConfig::TopPadding + 20.f },
        ShopScreenConfig::TextColor);
    target.draw(title);

    sf::Text coins = makeText(m_font, "Coins: " + std::to_string(m_gameState.getCoins()),
        ShopScreenConfig::ButtonTextSize,
        { WindowConfig::Width - ShopScreenConfig::SidePadding - 280.f, ShopScreenConfig::TopPadding + 15.f },
        ShopScreenConfig::TextColor);
    target.draw(coins);
}

void ShopScreen::drawUncatalogedWarning(sf::RenderTarget& target) const {
    if (!m_shopManager.hasUncatalogedShopFiles())
        return;

    sf::CircleShape warning(ShopScreenConfig::WarningDotRadius);
    warning.setFillColor(ShopScreenConfig::WarningColor);
    warning.setPosition({
        WindowConfig::Width - ShopScreenConfig::SidePadding - ShopScreenConfig::WarningDotRadius * 2.f,
        ShopScreenConfig::TopPadding
    });
    target.draw(warning);
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
        drawCategoryTab(target, type, bounds);
    }
}

void ShopScreen::drawCategoryTab(sf::RenderTarget& target, ShopAssetType type, sf::FloatRect bounds) const {
    const sf::Color fill = type == m_shopManager.activeType()
        ? ShopScreenConfig::SelectedColor
        : ShopScreenConfig::PanelColor;

    TextButton button(m_font, shopAssetTypeToTitle(type), bounds);
    button.setColors(ButtonStyles::shopColors(fill));
    button.setMetrics(ButtonStyles::shopMetrics(ShopScreenConfig::ButtonTextSize, 18));
    target.draw(button);
}

void ShopScreen::drawTiles(sf::RenderTarget& target) const {
    const std::vector<const ShopAsset*> assets = m_shopManager.visibleAssets();
    for (std::size_t index = 0; index < assets.size(); ++index)
        drawTile(target, *assets[index], ShopManager::tileBounds(index), assets[index] == m_shopManager.hoveredAsset());
}

void ShopScreen::drawTile(sf::RenderTarget& target,
                          const ShopAsset& asset,
                          sf::FloatRect bounds,
                          bool hovered) const {
    const bool owned = m_gameState.ownsAsset(asset);
    const bool selected = m_gameState.isAssetSelected(asset);
    drawTilePanel(target, bounds, selected, hovered);
    const sf::FloatRect imageBounds(
        { bounds.position.x + 8.f, bounds.position.y + 12.f },
        { bounds.size.x - 16.f, 244.f });
    m_preview.drawAssetArtwork(target, asset, imageBounds, owned, false);

    if (!owned)
        drawLockIcon(target, bounds);

    drawWrappedTileName(target,
        asset.title,
        { bounds.position.x + 22.f, bounds.position.y + 266.f },
        bounds.size.x - 44.f);
    drawTilePrice(target, asset, bounds, owned, selected);
}

void ShopScreen::drawTilePanel(sf::RenderTarget& target,
                               sf::FloatRect bounds,
                               bool selected,
                               bool hovered) const {
    sf::RectangleShape tile(bounds.size);
    tile.setPosition(bounds.position);
    const sf::Color fill = selected ? ShopScreenConfig::SelectedColor :
        (hovered ? ShopScreenConfig::PanelHoverColor : ShopScreenConfig::PanelColor);
    styleRectangle(tile, fill, ShopScreenConfig::OutlineColor, selected ? 4.f : 2.f);
    target.draw(tile);
}

void ShopScreen::drawLockIcon(sf::RenderTarget& target, sf::FloatRect tileBounds) const {
    sf::Sprite lock(ResourceManager::instance().getImage(GameObjects::Lock));
    const sf::FloatRect lockBounds(
        { tileBounds.position.x + tileBounds.size.x / 2.f - ShopScreenConfig::LockSize / 2.f,
          tileBounds.position.y + 95.f },
        { ShopScreenConfig::LockSize, ShopScreenConfig::LockSize });
    fitSpriteTo(lock, lockBounds);
    target.draw(lock);
}

void ShopScreen::drawTilePrice(sf::RenderTarget& target,
                               const ShopAsset& asset,
                               sf::FloatRect bounds,
                               bool owned,
                               bool selected) const {
    std::string price = selected ? "Selected" : (owned ? "Owned" : std::to_string(asset.price) + " coins");
    drawFittedText(target,
        price,
        ShopScreenConfig::TilePriceSize,
        14,
        { bounds.position.x + 22.f, bounds.position.y + 326.f },
        bounds.size.x - 44.f,
        selected ? ShopScreenConfig::TextColor : ShopScreenConfig::MutedTextColor);
}

void ShopScreen::drawWrappedTileName(sf::RenderTarget& target,
                                     const std::string& name,
                                     sf::Vector2f position,
                                     float maxWidth) const {
    const std::vector<std::string> lines = TextUtils::wrapTextLines(
        m_font,
        name,
        ShopScreenConfig::TileNameSize,
        maxWidth,
        2);

    for (std::size_t index = 0; index < lines.size(); ++index) {
        sf::Text line = makeText(m_font, lines[index],
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
    sf::Text text = makeText(m_font, value, size, position, color);
    while (text.getLocalBounds().size.x > maxWidth && text.getCharacterSize() > minSize)
        text.setCharacterSize(text.getCharacterSize() - 1u);
    target.draw(text);
}

#pragma endregion
