#pragma once

#include "Gameplay/LevelConfig.h"
#include "Gameplay/ShopData.h"
#include "Utilities/JsonUtils.h"

#include <filesystem>
#include <set>
#include <string>
#include <vector>

/// @brief Owns the game's JSON-backed data and refreshes it when source files change.
class DataManager {
public:
    // Public API for loading, reading, refreshing, and saving game data.

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
    // Private construction.

    /// @brief Private construction keeps DataManager as a single shared instance.
    DataManager() = default;

    // Private validation helpers.

    /// @brief Validates one parsed level and reports the stage number on failure.
    /// @param config Parsed level data to validate.
    /// @param stageIndex Zero-based stage index, used in error messages.
    /// @throws std::runtime_error if the level contains impossible gameplay values.
    static void validateLevel(const LevelConfig& config, std::size_t stageIndex);

    // Private game-data parsing helpers.

    /// @brief Reads the display title for a shop asset, with legacy "name" fallback.
    /// @param object JSON object to inspect.
    /// @param context Human-readable location used in error messages.
    /// @return Shop asset display title.
    static std::string requireTitle(const JsonUtils::Json& object, const std::string& context);

    /// @brief Converts one JSON stage object into a LevelConfig.
    /// @param value JSON object that describes one stage.
    /// @param stageIndex Zero-based stage index, used in error messages.
    /// @return Parsed level configuration.
    static LevelConfig parseLevel(const JsonUtils::Json& value, std::size_t stageIndex);

    /// @brief Converts one JSON shop asset object into a validated ShopAsset.
    /// @param value JSON object that describes one shop item.
    /// @return Parsed shop asset.
    static ShopAsset parseShopAsset(const JsonUtils::Json& value);

    /// @brief Converts the top-level "stages" JSON array into level configs.
    /// @param root Root JSON object that owns the "stages" array.
    /// @param context Human-readable location used in error messages.
    /// @return Parsed level configurations.
    static std::vector<LevelConfig> parseLevels(const JsonUtils::Json& root, const std::string& context);

    /// @brief Converts the top-level "shopAssets" JSON array into the shop catalog.
    /// @param root Root JSON object that owns the "shopAssets" array.
    /// @return Parsed shop catalog.
    static std::vector<ShopAsset> parseShopAssets(const JsonUtils::Json& root);

    // Private shop catalog helpers.

    /// @brief Identifies generated companion files that should not appear in shop warnings.
    /// @param path File path to inspect.
    /// @return True when the file should be ignored by uncataloged-file checks.
    static bool isHiddenShopCompanionFile(const std::filesystem::path& path);

    /// @brief Counts shop media files that exist on disk but are not present in the catalog.
    /// @param catalog Current shop catalog.
    /// @return Number of uncataloged media files.
    static int countUncatalogedShopFiles(const std::vector<ShopAsset>& catalog);

    /// @brief Finds the default owned asset for one shop asset type.
    /// @param catalog Current shop catalog.
    /// @param type Asset type to search.
    /// @return Pointer to the default asset, or nullptr if none exists.
    static const ShopAsset* findDefaultAsset(const std::vector<ShopAsset>& catalog, ShopAssetType type);

    // Private user-data helpers.

    /// @brief Clamps and reconciles user-owned data against the current shop catalog.
    /// @param userData User data to normalize in place.
    /// @param catalog Current shop catalog.
    static void normalizeUserData(UserData& userData, const std::vector<ShopAsset>& catalog);

    /// @brief Reads a forgiving array of positive integer asset IDs.
    /// @param object JSON object to inspect.
    /// @param key Field name that may contain an array of IDs.
    /// @return Positive integer IDs found in the array.
    static std::set<int> parseIdArray(const JsonUtils::Json& object, const std::string& key);

    /// @brief Serializes normalized user data to the on-disk JSON shape.
    /// @param userData User data to serialize.
    /// @return JSON object ready to write to disk.
    static JsonUtils::Json userDataToJson(const UserData& userData);

    /// @brief Parses saved user data while ignoring malformed optional fields.
    /// @param root Root JSON value from the user data file.
    /// @return Parsed user data, with invalid optional fields omitted.
    static UserData parseUserData(const JsonUtils::Json& root);

    // Private file timestamp helpers used by runtime JSON refresh.

    /// @brief Reads the last-write time for a file without throwing on filesystem errors.
    /// @param path Path to inspect.
    /// @return Last-write time, or the default file_time_type value when unavailable.
    std::filesystem::file_time_type fileTime(const std::string& path) const;

    /// @brief Checks whether a file's last-write time differs from the cached time.
    /// @param path Path to inspect.
    /// @param knownTime Cached timestamp to compare and initialize when needed.
    /// @param hasKnownTime Tracks whether knownTime already contains a meaningful timestamp.
    /// @return True when the file timestamp changed after the previous check.
    bool hasFileChanged(const std::string& path,
                        std::filesystem::file_time_type& knownTime,
                        bool& hasKnownTime) const;

    /// @brief Stores the current last-write time for a JSON file.
    /// @param path Path to inspect.
    /// @param knownTime Cached timestamp destination.
    /// @param hasKnownTime Flag set to true after caching.
    void rememberFileTime(const std::string& path,
                          std::filesystem::file_time_type& knownTime,
                          bool& hasKnownTime) const;

    // Private cached state and file ownership.

    /// @brief Path to the app-owned game data JSON file.
    std::string m_gameDataPath;

    /// @brief Path to the user-owned save data JSON file.
    std::string m_userDataPath;

    /// @brief Cached validated app-owned data.
    GameData m_gameData;

    /// @brief Cached normalized user-owned save data.
    UserData m_userData;

    /// @brief Last remembered write time for the game data file.
    std::filesystem::file_time_type m_gameDataWriteTime{};

    /// @brief Last remembered write time for the user data file.
    std::filesystem::file_time_type m_userDataWriteTime{};

    /// @brief True after m_gameDataWriteTime has been initialized.
    bool m_hasGameDataWriteTime = false;

    /// @brief True after m_userDataWriteTime has been initialized.
    bool m_hasUserDataWriteTime = false;
};
