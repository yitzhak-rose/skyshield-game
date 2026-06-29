#pragma once

/// @brief Immutable level parameters used to initialize GameState and World.
struct LevelConfig {
    int enemyCount = 0;
    float tankChance = 0.f;
    float spawnInterval = 0.f;
    float batteryDrain = 0.f;
    int startAmmo = 0;
};
