#pragma once

#include "Enums/LevelResult.h"
#include "Gameplay/ShopData.h"

#include <map>
#include <set>

struct LevelConfig;

/// @brief Stores mutable data shared across the game and current level.
class GameState {
public:
    /// @brief Initializes all level-specific data from a level configuration.
    void startLevel(const LevelConfig& config, int levelNumber = 1);

    /// @brief Drains battery according to the configured rate and elapsed time.
    void updateBattery(float deltaTime);

    /// @brief Uses one bomb if one is available.
    bool consumeBomb();

    /// @brief Applies damage to the command vehicle.
    void damageVehicle(float amount);

    /// @brief Records a defeated enemy and awards its coins.
    void recordEnemyDefeated(int coinReward);

    /// @brief Tracks a bomb that is currently falling (used to detect an unwinnable state).
    void addActiveBomb() { m_activeBombs++; }

    /// @brief Drops the count of falling bombs by one.
    void removeActiveBomb() { if (m_activeBombs > 0) --m_activeBombs; }

    int getLevelNumber() const { return m_levelNumber; }
    int getCoins() const { return m_coins; }
    int getLevelCoins() const { return m_levelCoins; }
    int getBombsAvailable() const { return m_bombsAvailable; }
    int getActiveBombs() const { return m_activeBombs; }
    float getBatteryPercent() const { return m_batteryPercent; }
    float getVehicleArmorPercent() const { return m_vehicleArmorPercent; }
    float getDamageTaken() const { return m_damageTaken; }
    int getEnemiesRemaining() const { return m_enemiesRemaining; }
    int getEnemiesDefeated() const { return m_enemiesDefeated; }
    LevelResult getLevelResult() const { return m_levelResult; }

    void setLevelResult(LevelResult result) { m_levelResult = result; }
    void setCoins(int coins);
    void setUserData(const UserData& userData);
    UserData getUserData() const;
    bool ownsAsset(const ShopAsset& asset) const;
    bool isAssetSelected(const ShopAsset& asset) const;
    bool canAfford(const ShopAsset& asset) const;
    bool buyAsset(const ShopAsset& asset);
    bool selectAsset(const ShopAsset& asset);
    int selectedAssetId(ShopAssetType type) const;

private:
    int m_levelNumber = 1;
    int m_coins = 0;
    int m_levelCoins = 0;
    int m_bombsAvailable = 0;
    int m_activeBombs = 0;
    float m_batteryPercent = 0.f;
    float m_batteryDrainRate = 0.f;
    float m_vehicleArmorPercent = 0.f;
    float m_damageTaken = 0.f;
    int m_enemiesRemaining = 0;
    int m_enemiesDefeated = 0;
    LevelResult m_levelResult = LevelResult::Running;
    std::map<ShopAssetType, std::set<int>> m_ownedAssets;
    std::map<ShopAssetType, int> m_selectedAssets;
};
