#include "App/Game.h"
#include "Screens/GameScreen.h"
#include "Resources/ResourceManager.h"
#include "Gameplay/ConfigLoader.h"
#include "Gameplay/Constants.h"
#include "Enums/LevelResult.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode({ static_cast<unsigned int>(WindowConfig::Width),
                            static_cast<unsigned int>(WindowConfig::Height) }),
        "SkyShield")
    , m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::Background))
    , m_stages(ConfigLoader::load("stages.txt")) {
    window.setFramerateLimit(60);
    initBackground();
    startStage(0);
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

void Game::startStage(int index) {
    m_currentStage = index;
    m_screen = std::make_unique<GameScreen>(
        m_stages[static_cast<std::size_t>(index)],
        m_gameState,
        [this]() { window.close(); });
}

void Game::advanceStage() {
    if (m_currentStage + 1 < static_cast<int>(m_stages.size()))
        startStage(m_currentStage + 1);
    else
        window.close(); // last stage cleared — victory screen comes later
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

    if (m_screen->isFinished()) {
        switch (m_screen->getResult()) {
            case LevelResult::Won:
                std::cout << "Won" << std::endl;
                advanceStage();
                break;
            case LevelResult::Lost:
                std::cout << "Lost" << std::endl;
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
