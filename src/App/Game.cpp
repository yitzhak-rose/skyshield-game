#include "App/Game.h"
#include "Screens/GameScreen.h"
#include "Screens/StartScreen.h"
#include "Resources/ResourceManager.h"
#include "Gameplay/Constants.h"
#include "Enums/LevelResult.h"

Game::Game()
    : window(sf::VideoMode({ static_cast<unsigned int>(WindowConfig::Width),
                            static_cast<unsigned int>(WindowConfig::Height) }),
        "SkyShield")
    , m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::Background)) {
    window.setFramerateLimit(60);
    initBackground();
    showStartScreen();
}

Game::~Game() = default;

void Game::initBackground() {
    const sf::Texture& texture = ResourceManager::instance().getImage(GameObjects::Background);
    const sf::Vector2u textureSize = texture.getSize();
    m_backgroundSprite.setScale({
        WindowConfig::Width / static_cast<float>(textureSize.x),
        WindowConfig::Height / static_cast<float>(textureSize.y)
        });
}

void Game::showGameScreen() {
    m_screen = std::make_unique<GameScreen>(
        m_gameState,
        [this]() { requestStartScreen(); });
}

void Game::showStartScreen() {
    m_startScreenRequested = false;
    m_screen = std::make_unique<StartScreen>([this]() { window.close(); });
}

void Game::requestStartScreen() {
    m_startScreenRequested = true;
}

void Game::run() {
    m_clock.restart();
    while (window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        m_screen->handleEvents(window);
        if (!window.isOpen())
            break;

        update(dt);
        if (!window.isOpen())
            break;

        render();
    }
}

void Game::update(float dt) {
    m_screen->update(dt);

    if (m_startScreenRequested) {
        showStartScreen();
        return;
    }

    if (m_screen->isFinished()) {
        switch (m_screen->getResult()) {
            case LevelResult::Won:
                showGameScreen();
                break;
            case LevelResult::Lost:
                break;
            case LevelResult::Running:
                break;
        }
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);
    m_screen->draw(window);
    window.display();
}
