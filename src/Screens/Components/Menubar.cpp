#include "Screens/Components/Menubar.h"
#include "App/AssetConstants.h"
#include "App/ScreenConstants.h"
#include "App/State/GameState.h"
#include "Resources/ResourceManager.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <cmath>
#include <string>

MenuBar::MenuBar(float height, GameState& gameState)
    : m_background({ WindowConfig::Width, height })
    , m_bombSprite(ResourceManager::instance().getImage(GameObjects::Bomb))
    , m_batterySprite(ResourceManager::instance().getImage(GameObjects::Battery))
    , m_coin(ResourceManager::instance().getImage(GameObjects::Coin),
        ResourceManager::instance().getFont(Fonts::StencilForce))
    , m_state(gameState)
    , m_font(ResourceManager::instance().getFont(Fonts::StencilForce))
    , m_levelFont(ResourceManager::instance().getFont(Fonts::BattleGrid))
    , m_width(WindowConfig::Width)
    , m_height(height) {
    configureBackground();
    configureBombSprite();
    configureBatterySprite();
    configureCoin();
}

void MenuBar::update(float dt) {
    m_coin.update(dt);
    m_coin.setCount(m_state.getCoins());
}

void MenuBar::configureBackground() {
    m_background.setFillColor(MenuBarConfig::BackgroundColor);
}

void MenuBar::configureBombSprite() {
    const sf::FloatRect bombBounds = m_bombSprite.getLocalBounds();
    GraphicsUtils::centerOrigin(m_bombSprite);
    m_bombSprite.setRotation(sf::degrees(180.f));
    m_bombSprite.setScale({ MenuBarConfig::IconSize / bombBounds.size.x,
                            MenuBarConfig::IconSize / bombBounds.size.y });
    m_bombSprite.setPosition({
        MenuBarConfig::SidePadding + MenuBarConfig::IconSize / 2.f,
        m_height / 2.f
        });
}

void MenuBar::configureBatterySprite() {
    m_batterySprite.setTextureRect(batteryFrameRect());
    GraphicsUtils::centerOrigin(m_batterySprite);

    const sf::FloatRect batteryBounds = m_batterySprite.getLocalBounds();
    const float batteryScale = MenuBarConfig::BatteryHeight / batteryBounds.size.y;
    m_batterySprite.setScale({ batteryScale, batteryScale });
    m_batterySprite.setPosition({
        pauseButtonBounds().position.x -
            MenuBarConfig::PauseBatteryGap - batteryBounds.size.x * batteryScale / 2.f,
        m_height / 2.f
        });
}

void MenuBar::configureCoin() {
    m_coin.setPosition({ MenuBarConfig::CoinIconX, m_height / 2.f });
}

void MenuBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_background, states);
    drawContent(target, states);
}

void MenuBar::drawContent(sf::RenderTarget& target, sf::RenderStates states) const {
    drawBombCounter(target, states);
    drawVehicleArmorBar(target, states);
    drawTitle(target, states);
    drawBattery(target, states);
    drawLevelIndicator(target, states);
    drawPauseButton(target, states);
    m_coin.draw(target, states);
}

void MenuBar::drawBombCounter(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_bombSprite, states);
    target.draw(makeBombCounterText(), states);
}

sf::Text MenuBar::makeBombCounterText() const {
    sf::Text text(m_font, "x " + std::to_string(m_state.getBombsAvailable()), MenuBarConfig::TextSize);
    text.setFillColor(sf::Color::White);
    text.setPosition({
        MenuBarConfig::SidePadding + MenuBarConfig::IconSize + MenuBarConfig::BombTextGap,
        (m_height - MenuBarConfig::TextSize) / 2.f - 2.f
        });
    return text;
}

void MenuBar::drawTitle(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(makeTitleText(), states);
}

sf::Text MenuBar::makeTitleText() const {
    sf::Text title(m_font, "SkyShield", MenuBarConfig::TitleTextSize);
    title.setFillColor(MenuBarConfig::TitleColor);
    GraphicsUtils::centerOrigin(title);
    title.setPosition({ m_width / 2.f, m_height / 2.f });
    return title;
}

void MenuBar::drawVehicleArmorBar(sf::RenderTarget& target, sf::RenderStates states) const {
    const float armorPercent = std::clamp(m_state.getVehicleArmorPercent(), 0.f, VehicleConfig::StartArmor);

    target.draw(makeVehicleArmorTrack(), states);
    target.draw(makeVehicleArmorFill(armorPercent), states);
    target.draw(makeVehicleArmorOutline(), states);
    target.draw(makeVehicleArmorText(armorPercent), states);
}

sf::Vector2f MenuBar::vehicleArmorBarPosition() const {
    return {
        MenuBarConfig::VehicleArmorBarX,
        (m_height - MenuBarConfig::VehicleArmorBarHeight) / 2.f
    };
}

sf::RectangleShape MenuBar::makeVehicleArmorTrack() const {
    sf::RectangleShape track({
        MenuBarConfig::VehicleArmorBarWidth,
        MenuBarConfig::VehicleArmorBarHeight
        });
    track.setPosition(vehicleArmorBarPosition());
    track.setFillColor(MenuBarConfig::VehicleArmorTrackColor);
    return track;
}

sf::RectangleShape MenuBar::makeVehicleArmorFill(float armorPercent) const {
    sf::RectangleShape fill({
        MenuBarConfig::VehicleArmorBarWidth * (armorPercent / VehicleConfig::StartArmor),
        MenuBarConfig::VehicleArmorBarHeight
        });
    fill.setPosition(vehicleArmorBarPosition());
    fill.setFillColor(vehicleArmorFillColor(armorPercent));
    return fill;
}

sf::RectangleShape MenuBar::makeVehicleArmorOutline() const {
    sf::RectangleShape outline({
        MenuBarConfig::VehicleArmorBarWidth,
        MenuBarConfig::VehicleArmorBarHeight
        });
    outline.setPosition(vehicleArmorBarPosition());
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(MenuBarConfig::VehicleArmorOutlineColor);
    outline.setOutlineThickness(MenuBarConfig::VehicleArmorBarOutlineThickness);
    return outline;
}

sf::Color MenuBar::vehicleArmorFillColor(float armorPercent) const {
    if (armorPercent <= VehicleConfig::CriticalArmor)
        return MenuBarConfig::VehicleArmorCriticalColor;

    if (armorPercent <= VehicleConfig::StartArmor / 2.f)
        return MenuBarConfig::VehicleArmorWarningColor;

    return MenuBarConfig::VehicleArmorHealthyColor;
}

sf::Text MenuBar::makeVehicleArmorText(float armorPercent) const {
    sf::Text text(m_levelFont, "HP " + std::to_string(static_cast<int>(std::round(armorPercent))) + "%", MenuBarConfig::VehicleArmorTextSize);
    text.setFillColor(sf::Color::White);

    const sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.f,
        textBounds.position.y + textBounds.size.y / 2.f
        });
    text.setPosition({
        MenuBarConfig::VehicleArmorBarX + MenuBarConfig::VehicleArmorBarWidth / 2.f,
        m_height / 2.f - 1.f
        });
    return text;
}

void MenuBar::drawBattery(sf::RenderTarget& target, sf::RenderStates states) const {
    updateBatterySpriteForView(target.getView());
    target.draw(m_batterySprite, states);
}

void MenuBar::updateBatterySpriteForView(const sf::View& view) const {
    m_batterySprite.setTextureRect(batteryFrameRect());
    GraphicsUtils::centerOrigin(m_batterySprite);

    const float batteryHalfWidth = m_batterySprite.getGlobalBounds().size.x / 2.f;
    const float pauseLeft = pauseButtonBoundsForView(view).position.x;
    m_batterySprite.setPosition({
        pauseLeft - MenuBarConfig::PauseBatteryGap - batteryHalfWidth,
        m_height / 2.f
        });
}

sf::IntRect MenuBar::batteryFrameRect() const {
    const sf::Vector2u textureSize = m_batterySprite.getTexture().getSize();
    const int frameWidth = static_cast<int>(textureSize.x) / BatteryConfig::FrameCount;
    const int frameHeight = static_cast<int>(textureSize.y);
    const float maxBattery = std::max(m_state.getMaxBatteryPercent(), BatteryConfig::FullPercent);
    const float batteryPercent = std::clamp(
        m_state.getBatteryPercent() / maxBattery * BatteryConfig::FullPercent,
        0.f,
        BatteryConfig::FullPercent);
    const float emptyPercent = BatteryConfig::FullPercent - batteryPercent;
    const float frameStep = BatteryConfig::FullPercent / static_cast<float>(BatteryConfig::FrameCount - 1);
    const int frameIndex = std::clamp(static_cast<int>(std::round(emptyPercent / frameStep)),
        0, BatteryConfig::FrameCount - 1);

    return sf::IntRect({ frameIndex * frameWidth, 0 }, { frameWidth, frameHeight });
}

void MenuBar::drawLevelIndicator(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(makeLevelText(), states);
}

sf::Text MenuBar::makeLevelText() const {
    sf::Text text(m_levelFont, "Level " + std::to_string(m_state.getLevelNumber()), MenuBarConfig::LevelTextSize);
    text.setFillColor(MenuBarConfig::LevelTextColor);

    const sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({
        textBounds.position.x + textBounds.size.x,
        textBounds.position.y + textBounds.size.y / 2.f
        });

    const sf::FloatRect batteryBounds = m_batterySprite.getGlobalBounds();
    text.setPosition({
        batteryBounds.position.x - MenuBarConfig::LevelBatteryGap,
        m_height / 2.f
    });
    return text;
}

sf::FloatRect MenuBar::pauseButtonBounds() const {
    return pauseButtonBounds(m_width);
}

sf::FloatRect MenuBar::pauseButtonBounds(float rightEdge) const {
    return {
        { rightEdge - MenuBarConfig::SidePadding - MenuBarConfig::PauseButtonSize,
          (m_height - MenuBarConfig::PauseButtonSize) / 2.f },
        { MenuBarConfig::PauseButtonSize, MenuBarConfig::PauseButtonSize }
    };
}

void MenuBar::drawPauseButton(sf::RenderTarget& target, sf::RenderStates states) const {
    const sf::FloatRect bounds = pauseButtonBoundsForView(target.getView());
    target.draw(makePauseButtonShape(bounds), states);
    drawPauseIcon(target, states, bounds);
}

sf::FloatRect MenuBar::pauseButtonBoundsForView(const sf::View& view) const {
    return pauseButtonBounds(view.getCenter().x + view.getSize().x / 2.f);
}

sf::RectangleShape MenuBar::makePauseButtonShape(sf::FloatRect bounds) const {
    sf::RectangleShape button(bounds.size);
    button.setPosition(bounds.position);
    button.setFillColor(MenuBarConfig::PauseButtonColor);
    button.setOutlineColor(MenuBarConfig::PauseButtonOutlineColor);
    button.setOutlineThickness(MenuBarConfig::VehicleArmorBarOutlineThickness);
    return button;
}

void MenuBar::drawPauseIcon(sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect bounds) const {
    const sf::Vector2f center = {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
    const float step = MenuBarConfig::PauseIconWidth + MenuBarConfig::PauseIconGap;
    for (float offset : { -step / 2.f, step / 2.f }) {
        sf::RectangleShape bar({ MenuBarConfig::PauseIconWidth, MenuBarConfig::PauseIconHeight });
        bar.setOrigin({ MenuBarConfig::PauseIconWidth / 2.f, MenuBarConfig::PauseIconHeight / 2.f });
        bar.setPosition({ center.x + offset, center.y });
        bar.setFillColor(MenuBarConfig::PauseIconColor);
        target.draw(bar, states);
    }
}
