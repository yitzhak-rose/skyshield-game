#include "Screens/Components/ShopPreview.h"

#include "App/ButtonStyles.h"
#include "App/AssetConstants.h"
#include "App/ScreenConstants.h"
#include "Screens/Components/TextButton.h"
#include "Utilities/Utils.h"

ShopPreview::ShopPreview(GameState& gameState,
                         const ShopManager& shopManager,
                         const std::map<int, sf::Texture>& textures,
                         const std::map<int, sf::Font>& fonts,
                         const sf::Font& fallbackFont)
    : m_gameState(gameState)
    , m_shopManager(shopManager)
    , m_textures(textures)
    , m_fonts(fonts)
    , m_fallbackFont(fallbackFont) {
}

void ShopPreview::drawAssetArtwork(sf::RenderTarget& target,
                                   const ShopAsset& asset,
                                   sf::FloatRect bounds,
                                   bool owned,
                                   bool large) const {
    if (ShopManager::isImageAsset(asset)) {
        drawImageArtwork(target, asset, bounds, owned);
        return;
    }

    drawArtworkBox(target, bounds, owned);
    if (ShopManager::isFontAsset(asset))
        drawFontArtwork(target, asset, bounds, large);
    else
        drawMusicArtwork(target, bounds, large);
}

void ShopPreview::drawArtworkBox(sf::RenderTarget& target, sf::FloatRect bounds, bool owned) const {
    sf::RectangleShape box(bounds.size);
    box.setPosition(bounds.position);
    const sf::Color fill = owned ? sf::Color(30, 45, 55, 230) : sf::Color(30, 45, 55, 115);
    Screen::styleRectangle(box, fill, ShopScreenConfig::OutlineColor, 2.f);
    target.draw(box);
}

void ShopPreview::drawImageArtwork(sf::RenderTarget& target,
                                   const ShopAsset& asset,
                                   sf::FloatRect bounds,
                                   bool owned) const {
    const auto texture = m_textures.find(asset.id);
    if (texture == m_textures.end())
        return;

    sf::Sprite sprite(texture->second);
    cropFirstShopFrame(sprite, asset.type);
    Screen::fitSpriteTo(sprite, bounds);

    if (!owned)
        sprite.setColor(ShopScreenConfig::LockedColor);

    target.draw(sprite);
}

void ShopPreview::drawFontArtwork(sf::RenderTarget& target,
                                  const ShopAsset& asset,
                                  sf::FloatRect bounds,
                                  bool large) const {
    const auto font = m_fonts.find(asset.id);
    const sf::Font& previewFont = font == m_fonts.end() ? m_fallbackFont : font->second;
    sf::Text sample(previewFont, large ? "Aa 123" : "Aa", large ? 46u : 54u);
    sample.setFillColor(ShopScreenConfig::TextColor);
    fitFontSample(sample, bounds);
    GraphicsUtils::centerOrigin(sample);
    sample.setPosition(Screen::centerOf(bounds));
    target.draw(sample);
}

void ShopPreview::fitFontSample(sf::Text& sample, sf::FloatRect bounds) {
    while ((sample.getLocalBounds().size.x > bounds.size.x - 24.f ||
            sample.getLocalBounds().size.y > bounds.size.y - 24.f) &&
           sample.getCharacterSize() > 24u) {
        sample.setCharacterSize(sample.getCharacterSize() - 2u);
    }
}

void ShopPreview::drawMusicArtwork(sf::RenderTarget& target, sf::FloatRect bounds, bool large) const {
    sf::Text music = Screen::makeCenteredText(m_fallbackFont, large ? "Music Preview" : "Music",
        large ? OverlayViewConfig::BodyTextSize : ShopScreenConfig::ButtonTextSize,
        Screen::centerOf(bounds),
        ShopScreenConfig::TextColor);
    target.draw(music);
}

void ShopPreview::draw(sf::RenderTarget& target) const {
    const ShopAsset* previewAsset = m_shopManager.previewAsset();
    if (!previewAsset)
        return;

    const sf::FloatRect panelBounds = drawPreviewPanel(target);
    drawPreviewTitle(target, *previewAsset, panelBounds);
    drawAssetArtwork(target, *previewAsset, previewArtworkBounds(*previewAsset, panelBounds), true, true);
    drawPreviewPrice(target, *previewAsset, panelBounds);
    drawAffordabilityWarning(target, *previewAsset, panelBounds);
    drawPreviewButtons(target, *previewAsset);
}

sf::FloatRect ShopPreview::drawPreviewPanel(sf::RenderTarget& target) const {
    sf::RectangleShape scrim({ WindowConfig::Width, WindowConfig::Height });
    scrim.setFillColor(OverlayViewConfig::backgroundDimColor);
    target.draw(scrim);

    const sf::FloatRect panelBounds(
        { WindowConfig::Width / 2.f - 430.f, WindowConfig::Height / 2.f - 290.f },
        { 860.f, 580.f });
    sf::RectangleShape panel(panelBounds.size);
    panel.setPosition(panelBounds.position);
    Screen::styleRectangle(panel,
        OverlayViewConfig::PanelColor,
        OverlayViewConfig::PanelOutlineColor,
        OverlayViewConfig::PanelOutlineThickness);
    target.draw(panel);
    return panelBounds;
}

void ShopPreview::drawPreviewTitle(sf::RenderTarget& target,
                                   const ShopAsset& asset,
                                   sf::FloatRect panelBounds) const {
    sf::Text title = Screen::makeCenteredText(m_fallbackFont, asset.title, OverlayViewConfig::TitleTextSize,
        { WindowConfig::Width / 2.f, panelBounds.position.y + 56.f },
        OverlayViewConfig::TitleColor);
    target.draw(title);
}

sf::FloatRect ShopPreview::previewArtworkBounds(const ShopAsset& asset, sf::FloatRect panelBounds) const {
    if (ShopManager::isImageAsset(asset)) {
        return {
            { panelBounds.position.x + 38.f, panelBounds.position.y + 105.f },
            { panelBounds.size.x - 76.f, 285.f }
        };
    }

    return {
        { WindowConfig::Width / 2.f - 190.f, panelBounds.position.y + 110.f },
        { 380.f, 260.f }
    };
}

void ShopPreview::drawPreviewPrice(sf::RenderTarget& target,
                                   const ShopAsset& asset,
                                   sf::FloatRect panelBounds) const {
    const std::string price = m_gameState.ownsAsset(asset)
        ? "Owned"
        : "Price: " + std::to_string(asset.price) + " coins";
    sf::Text priceText = Screen::makeCenteredText(m_fallbackFont, price, OverlayViewConfig::BodyTextSize,
        { WindowConfig::Width / 2.f, panelBounds.position.y + 405.f },
        ShopScreenConfig::TextColor);
    target.draw(priceText);
}

void ShopPreview::drawAffordabilityWarning(sf::RenderTarget& target,
                                           const ShopAsset& asset,
                                           sf::FloatRect panelBounds) const {
    if (m_gameState.ownsAsset(asset) || m_gameState.canAfford(asset))
        return;

    sf::Text warning = Screen::makeCenteredText(m_fallbackFont, "Not enough coins", 24,
        { WindowConfig::Width / 2.f, panelBounds.position.y + 442.f },
        ShopScreenConfig::WarningColor);
    target.draw(warning);
}

void ShopPreview::drawPreviewButtons(sf::RenderTarget& target, const ShopAsset& asset) const {
    drawPreviewButton(target, m_shopManager.previewButtonBounds(PreviewAction::Primary),
        m_shopManager.primaryPreviewLabel(asset), m_shopManager.primaryPreviewEnabled(asset), true);

    if (ShopManager::isMusicAsset(asset)) {
        drawPreviewButton(target, m_shopManager.previewButtonBounds(PreviewAction::Try),
            "Try", true, false);
    }

    drawPreviewButton(target, m_shopManager.previewButtonBounds(PreviewAction::Cancel),
        "Cancel", true, false);
}

void ShopPreview::drawPreviewButton(sf::RenderTarget& target,
                                    sf::FloatRect bounds,
                                    const std::string& label,
                                    bool enabled,
                                    bool primary) const {
    TextButton button(m_fallbackFont, label, bounds);
    button.setColors(ButtonStyles::disabledOverlayColors(
        previewButtonColor(enabled, primary),
        OverlayViewConfig::DisabledButtonColor));
    button.setMetrics(ButtonStyles::overlayMetrics(OverlayViewConfig::ButtonTextSize, 18));
    button.setEnabled(enabled);
    target.draw(button);
}

sf::Color ShopPreview::previewButtonColor(bool enabled, bool primary) {
    if (!enabled)
        return OverlayViewConfig::DisabledButtonColor;

    return primary ? OverlayViewConfig::PrimaryButtonColor : OverlayViewConfig::DefaultButtonColor;
}

int ShopPreview::shopPreviewFrameCount(ShopAssetType type) {
    switch (type) {
        case ShopAssetType::CommandVehicle:
            return VehicleConfig::Sprite.frameCount;
        case ShopAssetType::Drone:
            return DroneConfig::Sprite.frameCount;
        case ShopAssetType::FastEnemy:
            return FastEnemyConfig::Stats.sprite.frameCount;
        case ShopAssetType::TankEnemy:
            return TankEnemyConfig::Stats.sprite.frameCount;
        case ShopAssetType::Background:
        case ShopAssetType::BackgroundMusic:
        case ShopAssetType::Font:
            return 1;
    }

    return 1;
}

void ShopPreview::cropFirstShopFrame(sf::Sprite& sprite, ShopAssetType type) {
    const int frameCount = shopPreviewFrameCount(type);
    if (frameCount <= 1)
        return;

    const sf::Vector2u size = sprite.getTexture().getSize();
    sprite.setTextureRect(sf::IntRect(
        { 0, 0 },
        { static_cast<int>(size.x / static_cast<unsigned int>(frameCount)),
          static_cast<int>(size.y) }));
}
