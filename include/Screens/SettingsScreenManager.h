#pragma once

#include "App/State/GameState.h"
#include "Enums/SettingsAction.h"
#include "Screens/HelpScreen.h"
#include "Screens/Core/Screen.h"
#include "Screens/SettingsScreen.h"

#include <memory>

class SettingsScreenManager : public Screen {
public:
    explicit SettingsScreenManager(GameState& gameState);

    void handleScreenEvent(const sf::Event& event, const ScreenEventContext& context) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& target) override;

private:
    void handleSettingsRequests();
    void handleSettingsAction(SettingsAction action);
    void setMuted(bool muted);
    void setMaxVolume(float maxVolume);
    void resetUserData();
    void saveUserData() const;
    void syncAudioSettings() const;
    void showHelp();
    void closeHelpIfRequested();

    GameState& m_gameState;
    SettingsScreen m_settingsScreen;
    std::unique_ptr<HelpScreen> m_helpScreen;
};
