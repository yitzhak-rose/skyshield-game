#include "Gameplay/GameState.h"

#include "Gameplay/Constants.h"
#include "Gameplay/LevelConfig.h"

#include <algorithm>

void GameState::startLevel(const LevelConfig& config, int levelNumber) {
    m_levelNumber = std::max(1, levelNumber);
    m_levelCoins = 0;
    m_bombsAvailable = std::max(0, config.startAmmo);
    m_batteryPercent = DroneConfig::StartBattery;
    m_batteryDrainRate = std::max(0.f, config.batteryDrain);
    m_vehicleArmorPercent = VehicleConfig::StartArmor;
    m_damageTaken = 0.f;
    m_enemiesRemaining = std::max(0, config.enemyCount);
    m_enemiesDefeated = 0;
    m_levelResult = LevelResult::Running;
}

void GameState::updateBattery(float deltaTime) {
    const float drain = m_batteryDrainRate * std::max(0.f, deltaTime);
    m_batteryPercent = std::max(0.f, m_batteryPercent - drain);
}

bool GameState::consumeBomb() {
    if (m_bombsAvailable <= 0)
        return false;

    --m_bombsAvailable;
    return true;
}

void GameState::damageVehicle(float amount) {
    const float previousArmor = m_vehicleArmorPercent;
    m_vehicleArmorPercent = std::max(0.f, previousArmor - std::max(0.f, amount));
    m_damageTaken += previousArmor - m_vehicleArmorPercent;
}

void GameState::recordEnemyDefeated(int coinReward) {
    const int reward = std::max(0, coinReward);
    m_coins += reward;
    m_levelCoins += reward;
    ++m_enemiesDefeated;
    m_enemiesRemaining = std::max(0, m_enemiesRemaining - 1);
}
