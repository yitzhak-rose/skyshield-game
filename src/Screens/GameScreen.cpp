#include "Screens/GameScreen.h"
#include "Resources/DataManager.h"
#include "Resources/SoundManager.h"
#include "App/ScreenConstants.h"
#include "Utilities/Utils.h"
#include <string>
#include <utility>
#include <vector>

GameScreen::GameScreen(GameState& gameState, const std::vector<LevelConfig>& stages)
    : m_gameState(gameState)
    , m_stages(stages)
    , m_menuBar(std::make_unique<MenuBar>(MenuBarConfig::Height, gameState)) {
    startCurrentStage();
}

void GameScreen::update(float deltaTime) {
    if (updateOverlay(deltaTime))
        return;

    updateRunningLevel(deltaTime);
    handleLevelResult();
}

void GameScreen::updateRunningLevel(float deltaTime) {
    m_world->update(deltaTime);
    m_menuBar->update(deltaTime);
}

void GameScreen::handleLevelResult() {
    if (m_world->getResult() == LevelResult::Lost)
        showGameOverOverlay();

    if (m_world->getResult() == LevelResult::Won && !m_completionOverlayShown) {
        saveUserData();
        if (isFinalStage())
            showGameCompletedOverlay();
        else
            showLevelCompletedOverlay();
    }
}

void GameScreen::saveUserData() const {
    DataManager::instance().updateUserData(m_gameState.getUserData());
    DataManager::instance().saveUserData();
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

bool GameScreen::isFinalStage() const {
    return m_currentStage + 1 >= static_cast<int>(m_stages.size());
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

void GameScreen::showPauseOverlay() {
    SoundManager::instance().reduceBackgroundVolume();
    showOverlay(std::make_unique<OverlayView>(
        "Paused",
        std::vector<std::string>{ "Game is paused" },
        std::vector<OverlayAction>{
            { "Resume", OverlayActionType::Primary,
                []() { SoundManager::instance().restoreBackgroundVolume(); } },
            { "Go to Start", OverlayActionType::Danger,
              [this]() {
                  SoundManager::instance().restoreBackgroundVolume();
                  requestCommand(ScreenCommand::ReturnToStart);
              } }
    }));
}

bool GameScreen::handlePauseInput(const sf::Event& event, const ScreenEventContext& context) {
    if (isKeyPressed(event, sf::Keyboard::Key::Escape) || isPauseClick(event, context)) {
        showPauseOverlay();
        return true;
    }

    return false;
}

bool GameScreen::isPauseClick(const sf::Event& event, const ScreenEventContext& context) const {
    const float rightEdge = context.viewBounds.position.x + context.viewBounds.size.x;
    return isLeftMousePressed(event) &&
        m_menuBar &&
        GeometryUtils::contains(m_menuBar->pauseButtonBounds(rightEdge), context.mousePosition);
}

std::vector<std::string> GameScreen::makeLevelSummaryLines() const {
    return {
        "Enemies defeated: " + std::to_string(m_gameState.getEnemiesDefeated()),
        "Coins earned: " + std::to_string(m_gameState.getLevelCoins())
    };
}

void GameScreen::advanceStage() {
    ++m_currentStage;
    startCurrentStage();
}

void GameScreen::startCurrentStage() {
    m_world = std::make_unique<World>(
        m_stages[static_cast<std::size_t>(m_currentStage)],
        m_gameState,
        m_currentStage + 1);
    m_completionOverlayShown = false;
}

void GameScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (handlePauseInput(event, context))
        return;
}

void GameScreen::draw(sf::RenderWindow& target) {
    m_world->draw(target);
    target.draw(*m_menuBar);
    drawOverlay(target);
}
