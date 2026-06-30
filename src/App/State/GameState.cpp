#include "App/State/GameState.h"

#include "App/AssetConstants.h"
#include "Structs/LevelConfig.h"

#include <algorithm>

void GameState::startLevel(const LevelConfig& config, int levelNumber) {
    m_levelNumber = std::max(1, levelNumber);
    m_levelCoins = 0;
    m_levelCoinsCommitted = false;
    m_bombsAvailable = std::max(0, config.startAmmo);
    m_activeBombs = 0;
    const float batteryMultiplier = 1.f + static_cast<float>(
        std::clamp(selectedAssetExtra(ShopAssetType::Drone), 0, 500)) / 100.f;
    m_maxBatteryPercent = DroneConfig::StartBattery * batteryMultiplier;
    m_batteryPercent = m_maxBatteryPercent;
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
    m_vehicleArmorPercent = std::max(0.f, previousArmor - adjustedVehicleDamage(amount));
    m_damageTaken += previousArmor - m_vehicleArmorPercent;
}

void GameState::recordEnemyDefeated(int coinReward) {
    const int reward = std::max(0, coinReward);
    m_levelCoins += reward;
    ++m_enemiesDefeated;
    m_enemiesRemaining = std::max(0, m_enemiesRemaining - 1);
}

void GameState::commitLevelCoins() {
    if (m_levelCoinsCommitted)
        return;

    m_coins += m_levelCoins;
    m_levelCoinsCommitted = true;
}

void GameState::setCoins(int coins) {
    m_coins = std::max(0, coins);
}

void GameState::setMuted(bool muted) {
    m_muted = muted;
}

void GameState::setMaxVolume(float maxVolume) {
    m_maxVolume = std::clamp(maxVolume, 0.f, 100.f);
}

void GameState::setUserData(const UserData& userData) {
    setCoins(userData.coins);
    setMuted(userData.muted);
    setMaxVolume(userData.maxVolume);
    m_ownedAssets = userData.ownedAssets;
    m_selectedAssets = userData.selectedAssets;
}

UserData GameState::getUserData() const {
    UserData userData;
    userData.coins = m_coins;
    userData.muted = m_muted;
    userData.maxVolume = m_maxVolume;
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

void GameState::setSelectedAssetExtras(const std::vector<ShopAsset>& catalog) {
    m_selectedExtras.clear();

    for (const ShopAsset& asset : catalog) {
        if (selectedAssetId(asset.type) == asset.id)
            m_selectedExtras[asset.type] = std::max(0, asset.extra);
    }
}

int GameState::selectedAssetExtra(ShopAssetType type) const {
    const auto found = m_selectedExtras.find(type);
    return found == m_selectedExtras.end() ? 0 : found->second;
}

float GameState::adjustedVehicleDamage(float amount) const {
    const float clampedAmount = std::max(0.f, amount);
    const float reduction = std::clamp(
        static_cast<float>(selectedAssetExtra(ShopAssetType::CommandVehicle)) / 100.f,
        0.f,
        1.f);

    return clampedAmount * (1.f - reduction);
}
