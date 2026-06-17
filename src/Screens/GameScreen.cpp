#include "Screens/GameScreen.h"

GameScreen::GameScreen(GameState& gameState)
    : m_world(LevelConfig{2, 0.4f, 3.f, 4.f, 4}, gameState) {
}

void GameScreen::startGame() {
    
}

void GameScreen::draw(sf::RenderWindow& window) {
    m_world.draw(window);
}

void GameScreen::update(float deltaTime) {
    m_world.update(deltaTime);
}
