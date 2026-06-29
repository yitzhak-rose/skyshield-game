#include "App/State/GameState.h"

#include "App/AssetConstants.h"
#include "Structs/LevelConfig.h"

#include <algorithm>

void GameState::startLevel(const LevelConfig& config, int levelNumber) {
    m_levelNumber = std::max(1, levelNumber);
    m_levelCoins = 0;
    m_bombsAvailable = std::max(0, config.startAmmo);
    m_activeBombs = 0;
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

void GameState::setCoins(int coins) {
    m_coins = std::max(0, coins);
}

void GameState::setUserData(const UserData& userData) {
    setCoins(userData.coins);
    m_ownedAssets = userData.ownedAssets;
    m_selectedAssets = userData.selectedAssets;
}

UserData GameState::getUserData() const {
    UserData userData;
    userData.coins = m_coins;
    userData.ownedAssets = m_ownedAssets;
    userData.selectedAssets = m_selectedAssets;
    return userData;
}

bool GameState::ownsAsset(const ShopAsset& asset) const {
    if (asset.isDefault)
        return true;

    const auto found = m_ownedAssets.find(asset.type);
    return found != m_ownedAssets.end() && found->second.contains(asset.id);
}

bool GameState::isAssetSelected(const ShopAsset& asset) const {
    return selectedAssetId(asset.type) == asset.id;
}

bool GameState::canAfford(const ShopAsset& asset) const {
    return m_coins >= std::max(0, asset.price);
}

bool GameState::buyAsset(const ShopAsset& asset) {
    if (ownsAsset(asset))
        return selectAsset(asset);

    if (!canAfford(asset))
        return false;

    m_coins -= std::max(0, asset.price);
    m_ownedAssets[asset.type].insert(asset.id);
    m_selectedAssets[asset.type] = asset.id;
    return true;
}

bool GameState::selectAsset(const ShopAsset& asset) {
    if (!ownsAsset(asset))
        return false;

    m_ownedAssets[asset.type].insert(asset.id);
    m_selectedAssets[asset.type] = asset.id;
    return true;
}

int GameState::selectedAssetId(ShopAssetType type) const {
    const auto found = m_selectedAssets.find(type);
    return found == m_selectedAssets.end() ? 0 : found->second;
}
