#include "Screens/StartScreen.h"

#include "Gameplay/Constants.h"
#include "Gameplay/GameState.h"
#include "Resources/ResourceManager.h"
#include "Utilities/Utils.h"

#include <algorithm>
#include <memory>
#include <vector>

namespace {
bool contains(const sf::FloatRect& bounds, sf::Vector2f position) {
    return position.x >= bounds.position.x &&
        position.x <= bounds.position.x + bounds.size.x &&
        position.y >= bounds.position.y &&
        position.y <= bounds.position.y + bounds.size.y;
}

sf::Vector2f centerOf(const sf::RectangleShape& shape) {
    const sf::FloatRect bounds = shape.getGlobalBounds();
    return {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    };
}
}

StartScreen::StartScreen(const GameState& gameState)
    : m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::StartBackground))
    , m_settingsButton({ StartScreenConfig::SettingsButtonSize, StartScreenConfig::SettingsButtonSize })
    , m_settingsSprite(ResourceManager::instance().getImage(GameObjects::Gear))
    , m_coin(ResourceManager::instance().getImage(GameObjects::Coin),
        ResourceManager::instance().getFont(Fonts::StencilForce))
    , m_quitButton({ StartScreenConfig::QuitButtonSize, StartScreenConfig::QuitButtonSize })
    , m_quitSprite(ResourceManager::instance().getImage(GameObjects::XSquare))
    , m_startButton({ StartScreenConfig::ButtonWidth, StartScreenConfig::ButtonHeight })
    , m_shopButton({ StartScreenConfig::ButtonWidth, StartScreenConfig::ButtonHeight })
    , m_startText(ResourceManager::instance().getFont(Fonts::StencilForce),
                  "Start Game",
                  StartScreenConfig::ButtonTextSize)
    , m_shopText(ResourceManager::instance().getFont(Fonts::StencilForce),
                 "Shop",
                 StartScreenConfig::ButtonTextSize)
    , m_gameState(gameState) {
    configureLayout();
}

void StartScreen::configureLayout() {
    configureBackground();
    configureSettingsButton();
    configureCoinCounter();
    configureQuitButtonStyle();
    configureMainButtons();
    configureButtonText();
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
    m_settingsButton.setFillColor(StartScreenConfig::SettingsButtonColor);
    m_settingsButton.setOutlineColor(StartScreenConfig::ButtonOutlineColor);
    m_settingsButton.setOutlineThickness(StartScreenConfig::ButtonOutlineThickness);

    const sf::Vector2u settingsSize = m_settingsSprite.getTexture().getSize();
    const float settingsScale = StartScreenConfig::SettingsIconSize /
        static_cast<float>(std::max(settingsSize.x, settingsSize.y));
    m_settingsSprite.setScale({ settingsScale, settingsScale });
    GraphicsUtils::centerOrigin(m_settingsSprite);
    m_settingsSprite.setPosition(centerOf(m_settingsButton));
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
    m_quitButton.setFillColor(StartScreenConfig::QuitButtonColor);
    m_quitButton.setOutlineColor(StartScreenConfig::ButtonOutlineColor);
    m_quitButton.setOutlineThickness(StartScreenConfig::ButtonOutlineThickness);

    const sf::Vector2u quitSize = m_quitSprite.getTexture().getSize();
    const float quitScale = StartScreenConfig::QuitIconSize /
        static_cast<float>(std::max(quitSize.x, quitSize.y));
    m_quitSprite.setScale({ quitScale, quitScale });
    GraphicsUtils::centerOrigin(m_quitSprite);
    m_quitSprite.setColor(StartScreenConfig::QuitIconColor);
}

void StartScreen::configureMainButtons() {
    m_startButton.setPosition({
        WindowConfig::Width / 2.f - StartScreenConfig::ButtonWidth / 2.f,
        WindowConfig::Height / 2.f - StartScreenConfig::ButtonHeight -
            StartScreenConfig::ButtonGap / 2.f
        });
    m_shopButton.setPosition({
        WindowConfig::Width / 2.f - StartScreenConfig::ButtonWidth / 2.f,
        WindowConfig::Height / 2.f + StartScreenConfig::ButtonGap / 2.f
        });

    configureMenuButton(m_startButton);
    configureMenuButton(m_shopButton);
}

void StartScreen::configureMenuButton(sf::RectangleShape& button) {
    button.setFillColor(StartScreenConfig::ButtonColor);
    button.setOutlineColor(StartScreenConfig::ButtonOutlineColor);
    button.setOutlineThickness(StartScreenConfig::ButtonOutlineThickness);
}

void StartScreen::configureButtonText() {
    m_startText.setFillColor(StartScreenConfig::ButtonTextColor);
    m_shopText.setFillColor(StartScreenConfig::ButtonTextColor);
    GraphicsUtils::centerOrigin(m_startText);
    GraphicsUtils::centerOrigin(m_shopText);
    m_startText.setPosition(centerOf(m_startButton));
    m_shopText.setPosition(centerOf(m_shopButton));
}

void StartScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (event.is<sf::Event::MouseMoved>()) {
        updateHover(context.mousePosition);
    }

    if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePress->button == sf::Mouse::Button::Left)
            handleClick(context.mousePosition);
    }

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Enter ||
            key->code == sf::Keyboard::Key::Space)
            requestCommand(ScreenCommand::StartGame);
    }
}

void StartScreen::updateHover(sf::Vector2f position) {
    m_hoveredButton = buttonAt(position);
}

void StartScreen::handleClick(sf::Vector2f position) {
    switch (buttonAt(position)) {
        case Button::Start:
            requestCommand(ScreenCommand::StartGame);
            break;
        case Button::Shop:
            requestCommand(ScreenCommand::OpenShop);
            break;
        case Button::Settings:
            showFeatureUnavailableOverlay();
            break;
        case Button::Quit:
            requestCommand(ScreenCommand::Quit);
            break;
        case Button::None:
            break;
    }
}

StartScreen::Button StartScreen::buttonAt(sf::Vector2f position) const {
    if (contains(m_settingsButton.getGlobalBounds(), position))
        return Button::Settings;

    if (contains(m_quitButton.getGlobalBounds(), position))
        return Button::Quit;

    if (contains(m_startButton.getGlobalBounds(), position))
        return Button::Start;

    if (contains(m_shopButton.getGlobalBounds(), position))
        return Button::Shop;

    return Button::None;
}

void StartScreen::showFeatureUnavailableOverlay() {
    showOverlay(std::make_unique<OverlayView>(
        "Feature Unavailable",
        std::vector<std::string>{ "This feature is not available yet." },
        std::vector<OverlayAction>{
            { "OK", OverlayActionType::Primary, []() {} }
        }));
}

void StartScreen::update(float deltaTime) {
    m_coin.update(deltaTime);
    m_coin.setCount(m_gameState.getCoins());
    updateOverlay(deltaTime);
}

void StartScreen::draw(sf::RenderWindow& target) {
    target.draw(m_backgroundSprite);
    drawTextButton(target, sf::RenderStates::Default, m_startButton, m_startText, Button::Start);
    drawTextButton(target, sf::RenderStates::Default, m_shopButton, m_shopText, Button::Shop);
    drawSettingsButton(target);
    drawCoinCounter(target);
    drawQuitButton(target);
    drawOverlay(target);
}

void StartScreen::drawTextButton(sf::RenderWindow& target,
                                 sf::RenderStates states,
                                 const sf::RectangleShape& button,
                                 const sf::Text& text,
                                 Button buttonType) const {
    sf::RectangleShape buttonShape = button;
    if (m_hoveredButton == buttonType)
        buttonShape.setFillColor(StartScreenConfig::ButtonHoverColor);

    target.draw(buttonShape, states);
    target.draw(text, states);
}

void StartScreen::drawSettingsButton(sf::RenderWindow& target) const {
    sf::RectangleShape settingsButton = m_settingsButton;
    if (m_hoveredButton == Button::Settings)
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
    if (m_hoveredButton == Button::Quit)
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
    m_quitSprite.setPosition(centerOf(m_quitButton));
}

LevelResult StartScreen::getResult() const {
    return getCommand() == ScreenCommand::StartGame ? LevelResult::Won : LevelResult::Running;
}
