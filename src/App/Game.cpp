#include "App/Game.h"
#include "Screens/GameScreen.h"
#include "Screens/ShopScreen.h"
#include "Screens/StartScreen.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"
#include "Gameplay/Constants.h"
#include "Gameplay/DataManager.h"
#include "Enums/LevelResult.h"

#include <algorithm>
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

sf::Vector2u initialWindowSize() {
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    return {
        std::min(static_cast<unsigned int>(WindowConfig::Width), desktopMode.size.x),
        std::min(static_cast<unsigned int>(WindowConfig::Height), desktopMode.size.y)
    };
}
}

Game::Game()
    : window(sf::VideoMode(initialWindowSize()), "SkyShield")
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
    DataManager::instance().configure(GameDataPath, UserDataPath);
    DataManager::instance().load();
    m_gameState.setUserData(DataManager::instance().userData());
    syncShopSelections();
    m_lastSavedUserData = m_gameState.getUserData();
    DataManager::instance().setUserData(m_lastSavedUserData);
    DataManager::instance().saveUserData();
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
    m_screen = std::make_unique<GameScreen>(m_gameState, DataManager::instance().gameData().stages);
}

void Game::showShopScreen() {
    const GameData& gameData = DataManager::instance().gameData();
    m_activeScreen = ActiveScreen::Shop;
    m_screen = std::make_unique<ShopScreen>(
        m_gameState,
        gameData.shopAssets,
        gameData.uncatalogedShopFiles);
}

Game::~Game() {
    saveUserData();
}

void Game::saveUserData() {
    DataManager::instance().setUserData(m_gameState.getUserData());
    DataManager::instance().saveUserData();
    m_lastSavedUserData = m_gameState.getUserData();
}

void Game::syncShopSelections() {
    ResourceManager::instance().applyShopSelections(
        DataManager::instance().gameData().shopAssets,
        m_gameState.getUserData());
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

        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            const sf::FloatRect visibleArea(
                { 0.f, 0.f },
                { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });
            window.setView(sf::View(visibleArea));
        }

        m_screen->handleEvent(*event, makeEventContext(*event));
    }
}

ScreenEventContext Game::makeEventContext(const sf::Event& event) const {
    ScreenEventContext context;
    const sf::View& view = window.getView();
    context.viewBounds = {
        { view.getCenter().x - view.getSize().x / 2.f,
          view.getCenter().y - view.getSize().y / 2.f },
        view.getSize()
    };

    if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>())
        context.mousePosition = window.mapPixelToCoords(mouseMove->position, view);
    else if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>())
        context.mousePosition = window.mapPixelToCoords(mousePress->position, view);

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
    refreshDataIfChanged();
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

void Game::refreshDataIfChanged() {
    if (!DataManager::instance().refreshIfChanged())
        return;

    m_gameState.setUserData(DataManager::instance().userData());
    m_lastSavedUserData = m_gameState.getUserData();
    syncShopSelections();

    if (m_activeScreen == ActiveScreen::Shop)
        showShopScreen();
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);
    m_screen->draw(window);
    window.display();
}
