#include "App/Game.h"
#include "App/Constants.h"
#include "Enums/LevelResult.h"
#include "Resources/DataManager.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"
#include "Screens/GameScreen.h"
#include "Screens/SettingsScreenManager.h"
#include "Screens/ShopScreen.h"
#include "Screens/StartScreen.h"
#include "Utilities/AppUtils.h"

#include <exception>
#include <optional>
#include <string>
#include <vector>

#pragma region Public
Game::Game()
    : window(sf::VideoMode(AppUtils::initialWindowSize()), "SkyShield")
    , m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::Background)) {
    window.setFramerateLimit(60);
    initBackground();
    loadSavedData();
    showStartScreen();
}

Game::~Game() = default;

void Game::run() {
    m_clock.restart();
    while (window.isOpen()) {
        try {
            const float dt = m_clock.restart().asSeconds();

            if (m_fatalMode) {
                handleFatalEvents();
                if (window.isOpen())
                    renderFatalOverlay();
                continue;
            }

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
        } catch (const std::exception& error) {
            enterFatalMode(error.what());
            if (window.isOpen())
                renderFatalOverlay();
        }
    }
}
#pragma endregion

#pragma region Resource Loading
void Game::initBackground() {
    const sf::Texture& texture = ResourceManager::instance().getImage(GameObjects::Background);
    m_backgroundSprite.setTexture(texture, true);
    const sf::Vector2u textureSize = texture.getSize();
    m_backgroundSprite.setScale({
        WindowConfig::Width / static_cast<float>(textureSize.x),
        WindowConfig::Height / static_cast<float>(textureSize.y)
        });
}

void Game::loadSavedData() {
    DataManager::instance().load();
    m_gameState.setUserData(DataManager::instance().getUserData());
    ResourceManager::instance().applyUserDataSelections(m_gameState);
    SoundManager::instance().applyUserSettings(m_gameState.isMuted(), m_gameState.getMaxVolume());
    initBackground();
}

void Game::showStartScreen() {
    initBackground();
    SoundManager::instance().startStartBackground();
    replaceScreen(ActiveScreen::Start, std::make_unique<StartScreen>(m_gameState));
}

void Game::showGameScreen() {
    initBackground();
    SoundManager::instance().startBackground();
    replaceScreen(
        ActiveScreen::Game,
        std::make_unique<GameScreen>(m_gameState, DataManager::instance().getGameData().stages));
}

void Game::showShopScreen() {
    const GameData& gameData = DataManager::instance().getGameData();
    replaceScreen(ActiveScreen::Shop, std::make_unique<ShopScreen>(m_gameState, gameData.shopAssets));
}

void Game::replaceScreen(ActiveScreen activeScreen, std::unique_ptr<Screen> screen) {
    m_activeScreen = activeScreen;
    m_screen = std::move(screen);
}

#pragma endregion

#pragma region Event Handling
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
    else if (const auto* mouseRelease = event.getIf<sf::Event::MouseButtonReleased>())
        context.mousePosition = window.mapPixelToCoords(mouseRelease->position, view);

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
        case ScreenCommand::OpenSettings:
            return m_activeScreen == ActiveScreen::Start;
        case ScreenCommand::ReturnToStart:
            return m_activeScreen == ActiveScreen::Game ||
                m_activeScreen == ActiveScreen::Shop ||
                m_activeScreen == ActiveScreen::Settings;
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
        case ScreenCommand::OpenSettings:
            replaceScreen(ActiveScreen::Settings, std::make_unique<SettingsScreenManager>(m_gameState));
            break;
        case ScreenCommand::ReturnToStart:
            showStartScreen();
            break;
        case ScreenCommand::Quit:
            window.close();
            break;
        case ScreenCommand::None:
            break;
    }
}

void Game::enterFatalMode(const std::string& message) {
    if (m_fatalMode)
        return;

    m_fatalMode = true;
    SoundManager::instance().stopPreview();
    m_fatalOverlay = std::make_unique<OverlayView>(
        "Fatal Error",
        std::vector<std::string>{ message },
        std::vector<OverlayAction>{
            { "Quit", OverlayActionType::Danger, [this]() { window.close(); } }
        });
}

void Game::handleFatalEvents() {
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

        if (m_fatalOverlay)
            m_fatalOverlay->handleEvent(*event, makeEventContext(*event));
    }
}
#pragma endregion

#pragma region Updating
void Game::update(float dt) {
    m_screen->update(dt);
    SoundManager::instance().update(dt);
    handleScreenCommand();
}
#pragma endregion

#pragma region Drawing
void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);
    m_screen->draw(window);
    window.display();
}

void Game::renderFatalOverlay() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);

    if (m_screen)
        m_screen->draw(window);
    if (m_fatalOverlay)
        window.draw(*m_fatalOverlay);

    window.display();
}
#pragma endregion
