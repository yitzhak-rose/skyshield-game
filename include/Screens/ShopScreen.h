#pragma once

#include "Gameplay/GameState.h"
#include "Screens/Screen.h"

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

class ShopScreen : public Screen {
public:
    ShopScreen(GameState& gameState,
               const std::vector<ShopAsset>& catalog,
               int uncatalogedShopFiles);

    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& target) override;

private:
    enum class PreviewAction {
        None,
        Primary,
        Try,
        Cancel
    };

    void loadTextures();
    void configureLayout();
    void updateHover(sf::Vector2f position);
    void handleClick(sf::Vector2f position);
    void openPreview(const ShopAsset& asset);
    void closePreview();
    void activatePreviewAction(PreviewAction action);
    void saveUserData() const;
    bool isMusicAsset(const ShopAsset& asset) const;
    bool isFontAsset(const ShopAsset& asset) const;
    bool isImageAsset(const ShopAsset& asset) const;
    std::vector<const ShopAsset*> visibleAssets() const;
    const ShopAsset* tileAt(sf::Vector2f position) const;
    int categoryIndexAt(sf::Vector2f position) const;
    PreviewAction previewActionAt(sf::Vector2f position) const;
    std::string primaryPreviewLabel(const ShopAsset& asset) const;
    bool primaryPreviewEnabled(const ShopAsset& asset) const;
    void drawHeader(sf::RenderTarget& target) const;
    void drawCategories(sf::RenderTarget& target) const;
    void drawTiles(sf::RenderTarget& target) const;
    void drawTile(sf::RenderTarget& target,
                  const ShopAsset& asset,
                  sf::FloatRect bounds,
                  bool hovered) const;
    void drawAssetArtwork(sf::RenderTarget& target,
                          const ShopAsset& asset,
                          sf::FloatRect bounds,
                          bool owned,
                          bool large) const;
    void drawWrappedTileName(sf::RenderTarget& target,
                             const std::string& name,
                             sf::Vector2f position,
                             float maxWidth) const;
    void drawFittedText(sf::RenderTarget& target,
                        const std::string& value,
                        unsigned int size,
                        unsigned int minSize,
                        sf::Vector2f position,
                        float maxWidth,
                        sf::Color color) const;
    std::vector<std::string> wrapTextLines(const std::string& text,
                                           unsigned int size,
                                           float maxWidth,
                                           std::size_t maxLines) const;
    void drawPreview(sf::RenderTarget& target) const;
    void drawPreviewButton(sf::RenderTarget& target,
                           sf::FloatRect bounds,
                           const std::string& label,
                           bool enabled,
                           bool primary) const;
    sf::Text makeText(const std::string& value,
                      unsigned int size,
                      sf::Vector2f position,
                      sf::Color color) const;
    sf::FloatRect backButtonBounds() const;
    sf::FloatRect tileBounds(std::size_t index) const;
    sf::FloatRect previewButtonBounds(PreviewAction action) const;

    GameState& m_gameState;
    const std::vector<ShopAsset>& m_catalog;
    int m_uncatalogedShopFiles = 0;
    ShopAssetType m_activeType = ShopAssetType::CommandVehicle;
    const ShopAsset* m_hoveredAsset = nullptr;
    const ShopAsset* m_previewAsset = nullptr;
    mutable std::map<int, sf::Texture> m_textures;
    mutable std::map<int, sf::Font> m_fonts;
    sf::RectangleShape m_background;
    const sf::Font& m_font;
};
