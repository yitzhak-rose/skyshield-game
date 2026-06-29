#pragma once

#include "Structs/GameData.h"
#include "Structs/UserData.h"

/// @brief Public facade for cached typed game and user data.
class DataManager {
public:
#pragma region Public
    /// @brief Gets the shared data manager instance.
    static DataManager& instance();

    /// @brief Loads game data and user data immediately.
    void load();

    /// @brief Returns the currently cached app-owned data.
    const GameData& getGameData() const { return m_gameData; }

    /// @brief Returns the currently cached user-owned data.
    const UserData& getUserData() const { return m_userData; }

    /// @brief Replaces the cached user data and normalizes it against the current catalog.
    void updateUserData(const UserData& userData);

    /// @brief Saves the cached user data to the configured user data file.
    void saveUserData();

    /// @brief Returns a normalized save file containing only default user-owned data.
    UserData defaultUserData() const;
#pragma endregion

private:
#pragma region Private
    /// @brief Private construction keeps DataManager as a single shared instance.
    DataManager() = default;

    /// @brief Private destruction keeps lifetime tied to the Meyers singleton.
    ~DataManager() = default;

    DataManager(const DataManager&) = delete;
    DataManager(DataManager&&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager& operator=(DataManager&&) = delete;

    /// @brief Loads app-owned game data from the configured JSON file.
    /// @return Typed game data.
    GameData fetchGameData() const;

    /// @brief Loads user-owned data from the configured JSON file.
    /// @return Normalized typed user data.
    UserData fetchUserData() const;

    /// @brief Reconciles user-owned data with the currently loaded catalog.
    /// @param userData User data to normalize.
    /// @return Normalized user data.
    UserData normalizeUserData(const UserData& userData) const;

    /// @brief Writes typed user data to the configured JSON file.
    /// @param userData User data to persist.
    void writeUserData(const UserData& userData) const;

    /// @brief Cached app-owned data.
    GameData m_gameData;

    /// @brief Cached normalized user-owned save data.
    UserData m_userData;
#pragma endregion
};
