#pragma once

// Holds the tunable parameters of a single level, read from the config file.
struct LevelConfig {
    int   enemyCount;     // total enemies to spawn this level
    float tankChance;     // probability (0 to 1) that a spawned enemy is a tank
    float spawnInterval;  // seconds between enemy spawns
    float batteryDrain;   // battery percent drained per second
    int   startAmmo;      // bombs the drone starts the level with
};