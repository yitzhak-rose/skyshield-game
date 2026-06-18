#include "Gameplay/ConfigLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<LevelConfig> ConfigLoader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("ConfigLoader: cannot open stages file: " + path);

    std::vector<LevelConfig> stages;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        // Skip blank lines and comments (# ...).
        const std::size_t firstNonSpace = line.find_first_not_of(" \t\r");
        if (firstNonSpace == std::string::npos || line[firstNonSpace] == '#')
            continue;

        LevelConfig config = parseLine(line, lineNumber);
        validate(config, lineNumber);
        stages.push_back(config);
    }

    if (stages.empty())
        throw std::runtime_error("ConfigLoader: no valid stages found in: " + path);

    return stages;
}

LevelConfig ConfigLoader::parseLine(const std::string& line, int lineNumber) {
    std::istringstream stream(line);
    LevelConfig config{};

    stream >> config.enemyCount >> config.tankChance >> config.spawnInterval
        >> config.batteryDrain >> config.startAmmo;

    // The extraction fails if a field is missing or non-numeric.
    if (stream.fail())
        throw std::runtime_error("ConfigLoader: malformed line "
            + std::to_string(lineNumber) + " (expected 5 numbers): " + line);

    return config;
}

void ConfigLoader::validate(const LevelConfig& config, int lineNumber) {
    const auto fail = [lineNumber](const std::string& why) {
        throw std::runtime_error("ConfigLoader: line "
            + std::to_string(lineNumber) + " — " + why);
        };

    if (config.enemyCount < 1)                                fail("enemyCount must be >= 1");
    if (config.tankChance < 0.f || config.tankChance > 1.f)   fail("tankChance must be in [0, 1]");
    if (config.spawnInterval <= 0.f)                          fail("spawnInterval must be > 0");
    if (config.batteryDrain < 0.f)                            fail("batteryDrain must be >= 0");
    if (config.startAmmo < 1)                                 fail("startAmmo must be >= 1");
}