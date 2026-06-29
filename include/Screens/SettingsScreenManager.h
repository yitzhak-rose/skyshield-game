#pragma once

#include "App/State/GameState.h"
#include "Screens/Core/Screen.h"

#include <memory>

class SettingsScreenManager {
public:
    explicit SettingsScreenManager(GameState& gameState);

    std::unique_ptr<Screen> createSettingsScreen() const;
    std::unique_ptr<Screen> createHelpScreen() const;

private:
    GameState& m_gameState;
};
