#pragma once

// Holds immutable level parameters used to initialize GameState and World.
struct LevelConfig {
    int   enemyCount;     // total enemies to spawn this level
    float tankChance;     // probability (0 to 1) that a spawned enemy is a tank
    float spawnInterval;  // seconds between enemy spawns
    float batteryDrain;   // battery drain rate copied into GameState
    int   startAmmo;      // starting bombs copied into GameState
};
