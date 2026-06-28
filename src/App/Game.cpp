#include "App/Game.h"
#include "Screens/GameScreen.h"
#include "Screens/ShopScreen.h"
#include "Screens/StartScreen.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"
#include "Gameplay/Constants.h"
#include "Gameplay/DataManager.h"
#include "Enums/LevelResult.h"

#include <optional>
#include <string>

namespace {
const std::string GameDataPath = "data/gameData.json";
const std::string UserDataPath = "userData.json";

bool sameUserData(const UserData& lhs, const UserData& rhs) {
    return lhs.coins == rhs.coins &&
        lhs.ownedAssets == rhs.ownedAssets &&
        lhs.selectedAssets == rhs.selectedAssets;
}
}

Game::Game()
    : window(sf::VideoMode({ static_cast<unsigned int>(WindowConfig::Width),
                            static_cast<unsigned int>(WindowConfig::Height) }),
        "SkyShield")
    , m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::Background)) {
    window.setFramerateLimit(60);
    initBackground();
    loadSavedData();
    showStartScreen();
}

void Game::initBackground() {
    const sf::Texture& texture = ResourceManager::instance().getImage(GameObjects::Background);
    const sf::Vector2u textureSize = texture.getSize();
    m_backgroundSprite.setScale({
        WindowConfig::Width / static_cast<float>(textureSize.x),
        WindowConfig::Height / static_cast<float>(textureSize.y)
        });
}

void Game::loadSavedData() {
    m_gameData = DataManager::loadGameData(GameDataPath);
    UserData userData = DataManager::loadUserData(UserDataPath, m_gameData.shopAssets);
    m_gameState.setUserData(userData);
    syncShopSelections();
    m_lastSavedUserData = m_gameState.getUserData();
    DataManager::saveUserData(UserDataPath, m_lastSavedUserData);
}

void Game::showStartScreen() {
    m_activeScreen = ActiveScreen::Start;
    SoundManager::instance().startStartBackground();
    m_screen = std::make_unique<StartScreen>(m_gameState);
}

void Game::showGameScreen() {
    m_activeScreen = ActiveScreen::Game;
    syncShopSelections();
    SoundManager::instance().startBackground();
    m_screen = std::make_unique<GameScreen>(m_gameState, m_gameData.stages);
}

void Game::showShopScreen() {
    m_activeScreen = ActiveScreen::Shop;
    m_screen = std::make_unique<ShopScreen>(
        m_gameState,
        m_gameData.shopAssets,
        m_gameData.uncatalogedShopFiles);
}

Game::~Game() {
    saveUserData();
}

void Game::saveUserData() {
    DataManager::saveUserData(UserDataPath, m_gameState.getUserData());
    m_lastSavedUserData = m_gameState.getUserData();
}

void Game::syncShopSelections() {
    ResourceManager::instance().applyShopSelections(m_gameData.shopAssets, m_gameState.getUserData());
    m_backgroundSprite.setTexture(ResourceManager::instance().getImage(GameObjects::Background), true);
    initBackground();
}

void Game::run() {
    m_clock.restart();
    while (window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        handleEvents();
        if (!window.isOpen())
            break;

        handleScreenCommand();
        if (!window.isOpen())
            break;

        update(dt);
        if (!window.isOpen())
            break;

        render();
    }
}

void Game::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            break;
        }

        m_screen->handleEvent(*event, makeEventContext(*event));
    }
}

ScreenEventContext Game::makeEventContext(const sf::Event& event) const {
    ScreenEventContext context;

    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>())
        context.mousePosition = window.mapPixelToCoords(mouseMove->position, window.getDefaultView());
    else if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>())
        context.mousePosition = window.mapPixelToCoords(mousePress->position, window.getDefaultView());

    return context;
}

void Game::handleScreenCommand() {
    const ScreenCommand command = m_screen->getCommand();
    m_screen->clearCommand();

    if (!isCommandAllowed(command))
        return;

    applyScreenCommand(command);
}

bool Game::isCommandAllowed(ScreenCommand command) const {
    switch (command) {
        case ScreenCommand::StartGame:
            return m_activeScreen == ActiveScreen::Start;
        case ScreenCommand::OpenShop:
            return m_activeScreen == ActiveScreen::Start;
        case ScreenCommand::ReturnToStart:
            return m_activeScreen == ActiveScreen::Game || m_activeScreen == ActiveScreen::Shop;
        case ScreenCommand::Quit:
            return m_activeScreen == ActiveScreen::Start;
        case ScreenCommand::None:
            return true;
    }

    return false;
}

void Game::applyScreenCommand(ScreenCommand command) {
    switch (command) {
        case ScreenCommand::StartGame:
            showGameScreen();
            break;
        case ScreenCommand::OpenShop:
            showShopScreen();
            break;
        case ScreenCommand::ReturnToStart:
            syncShopSelections();
            showStartScreen();
            break;
        case ScreenCommand::Quit:
            window.close();
            break;
        case ScreenCommand::None:
            break;
    }
}

void Game::update(float dt) {
    m_screen->update(dt);
    saveUserDataIfChanged();
    SoundManager::instance().update(dt);
    handleScreenCommand();
}

void Game::saveUserDataIfChanged() {
    if (sameUserData(m_lastSavedUserData, m_gameState.getUserData()))
        return;

    saveUserData();
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);
    m_screen->draw(window);
    window.display();
}
