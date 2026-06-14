/*#include "Game.h"
#include "ResourceManager.h"
#include "Constants.h"

Game::Game()
    : window(sf::VideoMode({static_cast<unsigned int>(WindowConfig::Width),
                            static_cast<unsigned int>(WindowConfig::Height)}),
             "SFML Window"),
      m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::Background)) {
    window.setFramerateLimit(60);
    initBackground();
}

void Game::initBackground() {
    const sf::Texture& texture = ResourceManager::instance().getImage(GameObjects::Background);
    sf::Vector2u textureSize = texture.getSize();
    m_backgroundSprite.setScale({
        WindowConfig::Width / static_cast<float>(textureSize.x),
        WindowConfig::Height / static_cast<float>(textureSize.y)
    });
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);
    window.display();
}*/

#include "Game.h"
#include "ResourceManager.h"
#include "Constants.h"

// Temporary level parameters until the levels file (stages.txt) is read.
static LevelConfig makeDebugConfig() {
    return LevelConfig{
        .enemyCount = 10,
        .tankChance = 0.3f,
        .spawnInterval = 2.f,
        .batteryDrain = 1.5f,
        .startAmmo = 20
    };
}

Game::Game()
    : window(sf::VideoMode({ static_cast<unsigned int>(WindowConfig::Width),
                            static_cast<unsigned int>(WindowConfig::Height) }),
        "SkyShield"),
    m_backgroundSprite(ResourceManager::instance().getImage(GameObjects::Background)),
    m_world(makeDebugConfig()) {
    window.setFramerateLimit(60);
    initBackground();
}

void Game::initBackground() {
    const sf::Texture& texture = ResourceManager::instance().getImage(GameObjects::Background);
    sf::Vector2u textureSize = texture.getSize();
    m_backgroundSprite.setScale({
        WindowConfig::Width / static_cast<float>(textureSize.x),
        WindowConfig::Height / static_cast<float>(textureSize.y)
        });
}

void Game::run() {
    while (window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        processEvents();
        handleInput();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Space)
                m_world.dropBomb();
        }
    }
}

void Game::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        m_world.moveDroneLeft();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        m_world.moveDroneRight();
    else
        m_world.stopDrone();
}

void Game::update(float dt) {
    m_world.update(dt);
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.draw(m_backgroundSprite);
    m_world.draw(window);
    window.display();
}