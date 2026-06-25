#include "App/Game.h"
#include "Screens/GameScreen.h"
#include "Screens/StartScreen.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"
#include "Gameplay/Constants.h"
#include "Gameplay/DataManager.h"
#include "Enums/LevelResult.h"

#include <optional>
#include <string>

namespace {
const std::string SaveFilePath = "save.json";
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
   // m_gameState.setCoins(DataManager::loadCoins(SaveFilePath));
    m_lastSavedCoins = m_gameState.getCoins();
}

void Game::showStartScreen() {
    m_activeScreen = ActiveScreen::Start;
    SoundManager::instance().startStartBackground();
    m_screen = std::make_unique<StartScreen>(m_gameState);
}

void Game::showGameScreen() {
    m_activeScreen = ActiveScreen::Game;
    SoundManager::instance().startBackground();
    m_screen = std::make_unique<GameScreen>(m_gameState);
}

Game::~Game() {
    saveCoins();
}

void Game::saveCoins() {
    DataManager::saveCoins(SaveFilePath, m_gameState.getCoins());
    m_lastSavedCoins = m_gameState.getCoins();
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
            continue;
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
        case ScreenCommand::ReturnToStart:
            return m_activeScreen == ActiveScreen::Game;
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

void Game::update(float dt) {
    m_screen->update(dt);
    saveCoinsIfChanged();
    SoundManager::instance().update(dt);
    handleScreenCommand();
}

void Game::saveCoinsIfChanged() {
    if (m_lastSavedCoins == m_gameState.getCoins())
        return;

    saveCoins();
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);
    m_screen->draw(window);
    window.display();
}
