#include "Screens/GameScreen.h"

GameScreen::GameScreen(const LevelConfig& config, GameState& gameState)
    : m_world(config, gameState)
    , m_menuBar(std::make_unique<MenuBar>(MenuBarConfig::Height, gameState)) {
}

void GameScreen::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space)
            m_world.dropBomb();
    }
}

void GameScreen::update(float deltaTime) {
    handleInput();
    m_world.update(deltaTime);
    m_menuBar->update(deltaTime);
}

void GameScreen::draw(sf::RenderWindow& window) {
    m_world.draw(window);
    window.draw(*m_menuBar);
}

void GameScreen::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        m_world.moveDroneLeft();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        m_world.moveDroneRight();
    else
        m_world.stopDrone();
}