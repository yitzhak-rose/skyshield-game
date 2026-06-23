#pragma once

#include "Gameplay/LevelConfig.h"

#include <string>
#include <vector>

/// @brief Handles reading and writing the game's JSON data files.
class DataManager {
public:
    /// @brief Loads every level configuration from a JSON stages file.
    /// @param path Path to the JSON file that contains a top-level "stages" array.
    /// @return The validated level configurations in file order.
    /// @throws std::runtime_error if the file cannot be read or contains invalid level data.
    static std::vector<LevelConfig> loadLevels(const std::string& path);

    /// @brief Loads the saved coin total from a JSON save file.
    /// @param path Path to the JSON file that contains a top-level "coins" value.
    /// @return The saved coin total, or 0 when the file is missing or invalid.
    static int loadCoins(const std::string& path);

    /// @brief Saves the current coin total to a JSON save file.
    /// @param path Path to the save file to write.
    /// @param coins Coin total to save; negative values are clamped to 0.
    static void saveCoins(const std::string& path, int coins);

private:
    /// @brief Validates one parsed level and reports the stage number on failure.
    static void validateLevel(const LevelConfig& config, std::size_t stageIndex);
};
