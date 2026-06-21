#include "Screens/GameScreen.h"

#include <string>
#include <utility>
#include <vector>

GameScreen::GameScreen(const LevelConfig& config, GameState& gameState, std::function<void()> quitGame)
    : m_config(config)
    , m_gameState(gameState)
    , m_quitGame(std::move(quitGame))
    , m_world(std::make_unique<World>(m_config, m_gameState))
    , m_menuBar(std::make_unique<MenuBar>(MenuBarConfig::Height, gameState)) {
}

void GameScreen::handleEvent(const sf::Event& event) {
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space)
            m_world->dropBomb();
    }
}

void GameScreen::update(float deltaTime) {
    if (updateOverlay(deltaTime))
        return;

    handleInput();
    m_world->update(deltaTime);
    m_menuBar->update(deltaTime);

    if (m_world->getResult() == LevelResult::Lost)
        showGameOverOverlay();
}

void GameScreen::draw(sf::RenderWindow& window) {
    m_world->draw(window);
    window.draw(*m_menuBar);
    drawOverlay(window);
}

void GameScreen::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        m_world->moveDroneLeft();
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        m_world->moveDroneRight();
    else
        m_world->stopDrone();
}

void GameScreen::showGameOverOverlay() {
    showOverlay(std::make_unique<OverlayView>(
        "Game Over",
        std::vector<std::string>{
            "Enemies defeated: " + std::to_string(m_gameState.getEnemiesDefeated()),
            "Coins earned: " + std::to_string(m_gameState.getLevelCoins())
        },
        std::vector<OverlayAction>{
            { "Quit", OverlayActionType::Danger, m_quitGame },
            { "Restart", OverlayActionType::Primary, [this]() { restartLevel(); } }
        }));
}

void GameScreen::restartLevel() {
    m_world = std::make_unique<World>(m_config, m_gameState);
}
