#include "Screens/GameScreen.h"
#include "Resources/SoundManager.h"
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

    m_world->update(deltaTime);
    m_menuBar->update(deltaTime);

    // Trigger a screen shake whenever the vehicle takes fresh damage.
    float damage = m_gameState.getDamageTaken();
    if (damage > m_lastDamageTaken)
        m_shakeTimer = 0.3f;          // shake lasts 0.3 seconds
    m_lastDamageTaken = damage;

    updateShake(deltaTime);
    if (m_world->getResult() == LevelResult::Lost)
        showGameOverOverlay();

    if (m_world->getResult() == LevelResult::Won && !m_completionOverlayShown) {
        if (isFinalStage())
            showGameCompletedOverlay();
        else
            showLevelCompletedOverlay();
    }
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

void GameScreen::handleScreenEvent(const sf::Event& event, const ScreenEventContext& /*context*/) {
    m_world->handleEvent(event);
}

void GameScreen::draw(sf::RenderWindow& target) {
    sf::View original = target.getView();

    if (m_shakeTimer > 0.f) {
        float decay = m_shakeTimer / 0.3f;                       // 1 -> 0
        float dx = (std::rand() / (float)RAND_MAX * 2.f - 1.f) * 12.f * decay;
        float dy = (std::rand() / (float)RAND_MAX * 2.f - 1.f) * 12.f * decay;
        sf::View shaken = original;
        shaken.move({ dx, dy });
        target.setView(shaken);
    }

    m_world->draw(target);          // the world shakes

    target.setView(original);       // restore: HUD + overlay stay steady
    target.draw(*m_menuBar);
    drawOverlay(target);
}

void GameScreen::restartLevel() {
    startCurrentStage();
}

void GameScreen::updateShake(float dt) {
    if (m_shakeTimer > 0.f)
        m_shakeTimer -= dt;
}