#pragma once

#include "Drone.h"
#include "CommandVehicle.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Bullet.h"
#include "LevelConfig.h"
#include "Enums/LevelResult.h"

#include <vector>
#include <memory>

// The game's logic core: owns every gameplay object, runs the simulation,
// resolves collisions, and reports the stage outcome.
class World {
public:

    // Builds the level: creates the drone and vehicle, applies the level config.
    World(const LevelConfig& config);

    // Advances the whole simulation by one frame.
    void update(float dt);

    // Draws every object in the level.
    void draw(sf::RenderWindow& window) const;

    // Steers the player's drone (called by PlayState from input).
    void moveDroneLeft() { m_drone.moveLeft(); }
    void moveDroneRight() { m_drone.moveRight(); }
    void stopDrone() { m_drone.stop(); }

    // Drops a bomb if the drone still has ammo.
    void dropBomb();

    // Returns whether the level is running, won, or lost.
    LevelResult getResult() const { return m_result; }

    // Returns the coins earned in this level.
    int getCoinsEarned() const { return m_coinsEarned; }

    // Read-only access for the HUD (battery, ammo, armor).
    const Drone& getDrone() const { return m_drone; }
    const CommandVehicle& getVehicle() const { return m_vehicle; }

private:

    // Spawns queued enemies over time.
    void spawnEnemies(float dt);

    // Creates a single enemy at a screen edge (type chosen by the tank chance).
    void spawnEnemy();

    // Feeds positions to enemies, then updates every object.
    void updateObjects(float dt);

    // Turns enemy fire requests into real bullets.
    void collectEnemyFire();

    // Resolves bomb-enemy and bullet-vehicle overlaps.
    void handleCollisions();

    // Removes dead objects and rewards coins for dead enemies.
    void processDead();

    // Decides if the level is now won or lost.
    void checkResult();

    // The level's fixed parameters (read-only after construction).
    LevelConfig m_config;

    // The player and the protected target.
    Drone m_drone;
    CommandVehicle m_vehicle;

    // All active gameplay objects.
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Bomb> m_bombs;
    std::vector<Bullet> m_bullets;

    // Spawn state that changes during the level.
    int   m_enemiesToSpawn;
    float m_spawnTimer = 0.f;

    // Level progress.
    int         m_coinsEarned = 0;
    LevelResult m_result = LevelResult::Running;
};