#include "Screens/StartScreen.h"

#include "App/ButtonStyles.h"
#include "App/Constants.h"
#include "App/ScreenConstants.h"
#include "App/State/GameState.h"
#include "Resources/ResourceManager.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <memory>
#include <vector>

StartScreen::StartScreen(const GameState& gameState)
    : m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::StartBackground))
    , m_settingsButton({ StartScreenConfig::SettingsButtonSize, StartScreenConfig::SettingsButtonSize })
    , m_settingsSprite(ResourceManager::instance().getImage(GameObjects::Gear))
    , m_coin(ResourceManager::instance().getImage(GameObjects::Coin),
        ResourceManager::instance().getFont(Fonts::StencilForce))
    , m_quitButton({ StartScreenConfig::QuitButtonSize, StartScreenConfig::QuitButtonSize })
    , m_quitSprite(ResourceManager::instance().getImage(GameObjects::XSquare))
    , m_startButton(ResourceManager::instance().getFont(Fonts::StencilForce), "Start Game")
    , m_shopButton(ResourceManager::instance().getFont(Fonts::StencilForce), "Shop")
    , m_gameState(gameState) {
    configureLayout();
}

void StartScreen::configureLayout() {
    configureBackground();
    configureSettingsButton();
    configureCoinCounter();
    configureQuitButtonStyle();
    configureMainButtons();
}

void StartScreen::configureBackground() {
    const sf::Vector2u backgroundSize = m_backgroundSprite.getTexture().getSize();
    m_backgroundSprite.setScale({
        WindowConfig::Width / static_cast<float>(backgroundSize.x),
        WindowConfig::Height / static_cast<float>(backgroundSize.y)
        });
}

void StartScreen::configureSettingsButton() {
    m_settingsButton.setPosition({
        StartScreenConfig::SettingsPadding,
        StartScreenConfig::SettingsPadding
        });
    GraphicsUtils::styleRectangle(m_settingsButton,
        StartScreenConfig::SettingsButtonColor,
        StartScreenConfig::ButtonOutlineColor,
        StartScreenConfig::ButtonOutlineThickness);

    const sf::Vector2u settingsSize = m_settingsSprite.getTexture().getSize();
    const float settingsScale = StartScreenConfig::SettingsIconSize /
        static_cast<float>(std::max(settingsSize.x, settingsSize.y));
    m_settingsSprite.setScale({ settingsScale, settingsScale });
    GraphicsUtils::centerOrigin(m_settingsSprite);
    m_settingsSprite.setPosition(GeometryUtils::centerOf(m_settingsButton));
}

void StartScreen::configureCoinCounter() {
    m_coin.setCount(m_gameState.getCoins());
    m_coin.setPosition({
        WindowConfig::Width - StartScreenConfig::SettingsPadding -
            StartScreenConfig::QuitButtonSize -
            StartScreenConfig::CoinCounterWidth,
        StartScreenConfig::SettingsPadding + StartScreenConfig::QuitButtonSize / 2.f
        });
}

void StartScreen::configureQuitButtonStyle() {
    GraphicsUtils::styleRectangle(m_quitButton,
        StartScreenConfig::QuitButtonColor,
        StartScreenConfig::ButtonOutlineColor,
        StartScreenConfig::ButtonOutlineThickness);

    const sf::Vector2u quitSize = m_quitSprite.getTexture().getSize();
    const float quitScale = StartScreenConfig::QuitIconSize /
        static_cast<float>(std::max(quitSize.x, quitSize.y));
    m_quitSprite.setScale({ quitScale, quitScale });
    GraphicsUtils::centerOrigin(m_quitSprite);
    m_quitSprite.setColor(StartScreenConfig::QuitIconColor);
}

void StartScreen::configureMainButtons() {
    m_startButton.setBounds({
        {
            WindowConfig::Width / 2.f - StartScreenConfig::ButtonWidth / 2.f,
            WindowConfig::Height / 2.f - StartScreenConfig::ButtonHeight -
                StartScreenConfig::ButtonGap / 2.f
        },
        { StartScreenConfig::ButtonWidth, StartScreenConfig::ButtonHeight }
    });
    m_shopButton.setBounds({
        {
            WindowConfig::Width / 2.f - StartScreenConfig::ButtonWidth / 2.f,
            WindowConfig::Height / 2.f + StartScreenConfig::ButtonGap / 2.f
        },
        { StartScreenConfig::ButtonWidth, StartScreenConfig::ButtonHeight }
    });

    m_startButton.setColors(ButtonStyles::startMenuColors());
    m_shopButton.setColors(ButtonStyles::startMenuColors());
    m_startButton.setMetrics(ButtonStyles::startMenuMetrics());
    m_shopButton.setMetrics(ButtonStyles::startMenuMetrics());
    m_startButton.setCallback([this]() { requestCommand(ScreenCommand::StartGame); });
    m_shopButton.setCallback([this]() { requestCommand(ScreenCommand::OpenShop); });
}

void StartScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (isMouseMoved(event))
        updateHover(context.mousePosition);

    if (isLeftMousePressed(event))
        handleClick(context.mousePosition);

    if (isAnyKeyPressed(event, { sf::Keyboard::Key::Enter, sf::Keyboard::Key::Space }))
        requestCommand(ScreenCommand::StartGame);
}

void StartScreen::updateHover(sf::Vector2f position) {
    m_hoveredButton = buttonAt(position);
    m_startButton.setHovered(m_hoveredButton == StartScreenButton::Start);
    m_shopButton.setHovered(m_hoveredButton == StartScreenButton::Shop);
}

void StartScreen::handleClick(sf::Vector2f position) {
    if (m_startButton.click(position) || m_shopButton.click(position))
        return;

    switch (buttonAt(position)) {
        case StartScreenButton::Start:
        case StartScreenButton::Shop:
            break;
        case StartScreenButton::Settings:
            requestCommand(ScreenCommand::OpenSettings);
            break;
        case StartScreenButton::Quit:
            requestCommand(ScreenCommand::Quit);
            break;
        case StartScreenButton::None:
            break;
    }
}

StartScreenButton StartScreen::buttonAt(sf::Vector2f position) const {
    if (GeometryUtils::contains(m_settingsButton.getGlobalBounds(), position))
        return StartScreenButton::Settings;

    if (GeometryUtils::contains(m_quitButton.getGlobalBounds(), position))
        return StartScreenButton::Quit;

    if (GeometryUtils::contains(m_startButton.bounds(), position))
        return StartScreenButton::Start;

    if (GeometryUtils::contains(m_shopButton.bounds(), position))
        return StartScreenButton::Shop;

    return StartScreenButton::None;
}

void StartScreen::update(float deltaTime) {
    m_coin.update(deltaTime);
    m_coin.setCount(m_gameState.getCoins());
    updateOverlay(deltaTime);
}

void StartScreen::draw(sf::RenderWindow& target) {
    target.draw(m_backgroundSprite);
    target.draw(m_startButton);
    target.draw(m_shopButton);
    drawSettingsButton(target);
    drawCoinCounter(target);
    drawQuitButton(target);
    drawOverlay(target);
}

void StartScreen::drawSettingsButton(sf::RenderWindow& target) const {
    sf::RectangleShape settingsButton = m_settingsButton;
    if (m_hoveredButton == StartScreenButton::Settings)
        settingsButton.setFillColor(StartScreenConfig::SettingsButtonHoverColor);

    target.draw(settingsButton);
    target.draw(m_settingsSprite);
}

void StartScreen::drawCoinCounter(sf::RenderWindow& target) {
    updateQuitButtonForTarget(target);
    const sf::FloatRect quitBounds = m_quitButton.getGlobalBounds();
    m_coin.setPosition({
        quitBounds.position.x - StartScreenConfig::CoinCounterWidth,
        quitBounds.position.y + quitBounds.size.y / 2.f
        });
    m_coin.draw(target);
}

void StartScreen::drawQuitButton(sf::RenderWindow& target) const {
    updateQuitButtonForTarget(target);

    sf::RectangleShape quitButton = m_quitButton;
    if (m_hoveredButton == StartScreenButton::Quit)
        quitButton.setFillColor(StartScreenConfig::QuitButtonHoverColor);

    target.draw(quitButton);
    target.draw(m_quitSprite);
}

void StartScreen::updateQuitButtonForTarget(const sf::RenderWindow& target) const {
    const sf::View view = target.getView();
    const float rightEdge = view.getCenter().x + view.getSize().x / 2.f;
    const float topEdge = view.getCenter().y - view.getSize().y / 2.f;

    m_quitButton.setPosition({
        rightEdge - StartScreenConfig::SettingsPadding - StartScreenConfig::QuitButtonSize,
        topEdge + StartScreenConfig::SettingsPadding
        });
    m_quitSprite.setPosition(GeometryUtils::centerOf(m_quitButton));
}
