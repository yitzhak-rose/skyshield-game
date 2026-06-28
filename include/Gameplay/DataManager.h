#pragma once

#include "Gameplay/LevelConfig.h"
#include "Gameplay/ShopData.h"

#include <filesystem>
#include <string>
#include <vector>

/// @brief Owns the game's JSON-backed data and refreshes it when source files change.
class DataManager {
public:
    /// @brief Gets the shared data manager instance.
    static DataManager& instance();

    /// @brief Sets the JSON files this manager owns.
    void configure(std::string gameDataPath, std::string userDataPath);

    /// @brief Loads game data and user data immediately.
    void load();

    /// @brief Reloads cached data when one of the JSON files was modified.
    /// @return True if game data or user data changed.
    bool refreshIfChanged();

    /// @brief Returns the currently cached app-owned data.
    const GameData& gameData() const { return m_gameData; }

    /// @brief Returns the currently cached user-owned data.
    const UserData& userData() const { return m_userData; }

    /// @brief Replaces the cached user data and normalizes it against the current catalog.
    void setUserData(const UserData& userData);

    /// @brief Saves the cached user data to the configured user-data file.
    void saveUserData();

    /// @brief Loads every level configuration from a JSON stages file.
    /// @param path Path to the JSON file that contains a top-level "stages" array.
    /// @return The validated level configurations in file order.
    /// @throws std::runtime_error if the file cannot be read or contains invalid level data.
    std::vector<LevelConfig> loadLevels(const std::string& path) const;

    /// @brief Loads stages and shop catalog from the app-owned game data file.
    /// @param path Path to the JSON file that contains "stages" and "shopAssets".
    /// @return Validated immutable game data.
    GameData loadGameData(const std::string& path) const;

    /// @brief Loads user-owned state and normalizes default ownership from the catalog.
    /// @param path Path to the JSON user data file.
    /// @param catalog Shop assets used to guarantee default ownership.
    /// @return User state, or defaults when the file is missing or invalid.
    UserData loadUserData(const std::string& path, const std::vector<ShopAsset>& catalog) const;

    /// @brief Saves user-owned state, including coins, owned assets, and selections.
    /// @param path Path to the user data file.
    /// @param userData User state to persist.
    void saveUserData(const std::string& path, const UserData& userData);

    /// @brief Loads the saved coin total from a JSON save file.
    /// @param path Path to the JSON file that contains a top-level "coins" value.
    /// @return The saved coin total, or 0 when the file is missing or invalid.
    int loadCoins(const std::string& path) const;

    /// @brief Saves the current coin total to a JSON save file.
    /// @param path Path to the save file to write.
    /// @param coins Coin total to save; negative values are clamped to 0.
    void saveCoins(const std::string& path, int coins);

private:
    DataManager() = default;

    /// @brief Validates one parsed level and reports the stage number on failure.
    static void validateLevel(const LevelConfig& config, std::size_t stageIndex);

    std::filesystem::file_time_type fileTime(const std::string& path) const;
    bool hasFileChanged(const std::string& path,
                        std::filesystem::file_time_type& knownTime,
                        bool& hasKnownTime) const;
    void rememberFileTime(const std::string& path,
                          std::filesystem::file_time_type& knownTime,
                          bool& hasKnownTime) const;

    std::string m_gameDataPath;
    std::string m_userDataPath;
    GameData m_gameData;
    UserData m_userData;
    std::filesystem::file_time_type m_gameDataWriteTime{};
    std::filesystem::file_time_type m_userDataWriteTime{};
    bool m_hasGameDataWriteTime = false;
    bool m_hasUserDataWriteTime = false;
};
