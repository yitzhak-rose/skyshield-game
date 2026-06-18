#pragma once
#include "Gameplay/LevelConfig.h"
#include <vector>
#include <string>

/// @brief Reads the stages file and turns each line into a LevelConfig.
class ConfigLoader {
public:
    /// @brief Loads every stage from the given file.
    static std::vector<LevelConfig> load(const std::string& path);

private:
    /// @brief Parses one line into a config; throws (with the line number) if invalid.
    static LevelConfig parseLine(const std::string& line, int lineNumber);

    /// @brief Validates a parsed config; throws (with the line number) if out of range.
    static void validate(const LevelConfig& config, int lineNumber);
};