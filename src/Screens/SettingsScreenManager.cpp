#include "Screens/SettingsScreenManager.h"

#include "Resources/DataManager.h"
#include "Resources/ResourceManager.h"
#include "Resources/SoundManager.h"

SettingsScreenManager::SettingsScreenManager(GameState& gameState)
    : m_gameState(gameState)
    , m_settingsScreen(gameState) {
    syncAudioSettings();
}

void SettingsScreenManager::handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) {
    if (m_helpScreen) {
        m_helpScreen->handleEvent(event, context);
        closeHelpIfRequested();
        return;
    }

    m_settingsScreen.handleEvent(event, context);
    handleSettingsRequests();
}

void SettingsScreenManager::update(float deltaTime) {
    if (!m_helpScreen)
        m_settingsScreen.update(deltaTime);

    handleSettingsRequests();
}

void SettingsScreenManager::draw(sf::RenderWindow& target) {
    if (m_helpScreen) {
        m_helpScreen->draw(target);
        return;
    }

    m_settingsScreen.draw(target);
}

void SettingsScreenManager::handleSettingsRequests() {
    float requestedVolume = 0.f;
    if (m_settingsScreen.consumeVolumeChange(requestedVolume))
        setMaxVolume(requestedVolume);

    handleSettingsAction(m_settingsScreen.consumeAction());
}

void SettingsScreenManager::handleSettingsAction(SettingsAction action) {
    switch (action) {
        case SettingsAction::ReturnToStart:
            requestCommand(ScreenCommand::ReturnToStart);
            break;
        case SettingsAction::Mute:
            setMuted(true);
            break;
        case SettingsAction::Unmute:
            setMuted(false);
            break;
        case SettingsAction::OpenHelp:
            showHelp();
            break;
        case SettingsAction::ResetUserData:
            resetUserData();
            break;
        case SettingsAction::None:
            break;
    }
}

void SettingsScreenManager::setMuted(bool muted) {
    m_gameState.setMuted(muted);
    syncAudioSettings();
    saveUserData();
}

void SettingsScreenManager::setMaxVolume(float maxVolume) {
    m_gameState.setMaxVolume(maxVolume);
    syncAudioSettings();
    saveUserData();
}

void SettingsScreenManager::resetUserData() {
    m_gameState.resetUserData(DataManager::instance().defaultUserData());
    ResourceManager::instance().applyUserDataSelections(m_gameState);
    syncAudioSettings();
    saveUserData();
}

void SettingsScreenManager::saveUserData() const {
    DataManager::instance().updateUserData(m_gameState.getUserData());
    DataManager::instance().saveUserData();
}

void SettingsScreenManager::syncAudioSettings() const {
    SoundManager::instance().applyUserSettings(m_gameState.isMuted(), m_gameState.getMaxVolume());
}

void SettingsScreenManager::showHelp() {
    m_helpScreen = std::make_unique<HelpScreen>();
}

void SettingsScreenManager::closeHelpIfRequested() {
    if (m_helpScreen && m_helpScreen->isCloseRequested())
        m_helpScreen.reset();
}
