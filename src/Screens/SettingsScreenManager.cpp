#include "Screens/SettingsScreenManager.h"

#include "Screens/SettingsScreen.h"

SettingsScreenManager::SettingsScreenManager(GameState& gameState)
    : m_gameState(gameState) {
}

std::unique_ptr<Screen> SettingsScreenManager::createSettingsScreen() const {
    return std::make_unique<SettingsScreen>(m_gameState);
}
