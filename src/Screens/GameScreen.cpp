#include "Screens/GameScreen.h"

#include "Gameplay/ConfigLoader.h"
#include "Resources/SoundManager.h"

#include <string>
#include <utility>
#include <vector>

GameScreen::GameScreen(GameState& gameState)
    : m_gameState(gameState)
    , m_stages(ConfigLoader::load("stages.txt"))
    , m_menuBar(std::make_unique<MenuBar>(MenuBarConfig::Height, gameState)) {
    startCurrentStage();
}

void GameScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& /*context*/) {
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

    if (m_world->getResult() == LevelResult::Won && !m_completionOverlayShown) {
        if (isFinalStage())
            showGameCompletedOverlay();
        else
            showLevelCompletedOverlay();
    }
}

void GameScreen::draw(sf::RenderTarget& target) {
    m_world->draw(target);
    target.draw(*m_menuBar);
    drawOverlay(target);
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
    SoundManager::instance().playMissionFailed();
    showOverlay(std::make_unique<OverlayView>(
        "Game Over",
        makeLevelSummaryLines(),
        std::vector<OverlayAction>{
            { "Go to Start", OverlayActionType::Primary, [this]() { requestCommand(ScreenCommand::ReturnToStart); } }
        }));
}

void GameScreen::showLevelCompletedOverlay() {
    m_completionOverlayShown = true;
    SoundManager::instance().playLevelFinished();
    showOverlay(std::make_unique<OverlayView>(
        "Level Completed",
        makeLevelSummaryLines(),
        std::vector<OverlayAction>{
            { "Next Level", OverlayActionType::Primary, [this]() { advanceStage(); } }
        }));
}

void GameScreen::showGameCompletedOverlay() {
    m_completionOverlayShown = true;
    SoundManager::instance().playLevelFinished();
    showOverlay(std::make_unique<OverlayView>(
        "Game Completed",
        makeLevelSummaryLines(),
        std::vector<OverlayAction>{
            { "Start Screen", OverlayActionType::Primary, [this]() { requestCommand(ScreenCommand::ReturnToStart); } }
        }));
}

void GameScreen::startCurrentStage() {
    m_world = std::make_unique<World>(
        m_stages[static_cast<std::size_t>(m_currentStage)],
        m_gameState,
        m_currentStage + 1);
    m_completionOverlayShown = false;
}

void GameScreen::advanceStage() {
    ++m_currentStage;
    startCurrentStage();
}

bool GameScreen::isFinalStage() const {
    return m_currentStage + 1 >= static_cast<int>(m_stages.size());
}

std::vector<std::string> GameScreen::makeLevelSummaryLines() const {
    return {
        "Enemies defeated: " + std::to_string(m_gameState.getEnemiesDefeated()),
        "Coins earned: " + std::to_string(m_gameState.getLevelCoins())
    };
}

void GameScreen::restartLevel() {
    startCurrentStage();
}
