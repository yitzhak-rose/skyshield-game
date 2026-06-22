#pragma once

#include "Gameplay/Objects/Drone.h"
#include "Gameplay/Objects/CommandVehicle.h"
#include "Gameplay/Enemies/Enemy.h"
#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/Projectiles/Bullet.h"
#include "Gameplay/GameState.h"
#include "Gameplay/LevelConfig.h"

#include <vector>
#include <memory>

// The game's logic core: owns every gameplay object, runs the simulation,
// resolves collisions, and reports the stage outcome.
class World {
public:

    // Builds the level: creates the drone and vehicle, applies the level config.
    World(const LevelConfig& config, GameState& gameState, int levelNumber);

    // Advances the whole simulation by one frame.
    void update(float dt);

    // Draws every object in the level.
    void draw(sf::RenderTarget& target) const;

    // Steers the player's drone (called by PlayState from input).
    void moveDroneLeft() { m_drone.moveLeft(); }
    void moveDroneRight() { m_drone.moveRight(); }
    void stopDrone() { m_drone.stop(); }

    // Drops a bomb if the drone still has ammo.
    void dropBomb();

    // Returns whether the level is running, won, or lost.
    LevelResult getResult() const { return m_gameState.getLevelResult(); }

    // Returns the coins earned in this level.
    int getCoinsEarned() const { return m_gameState.getLevelCoins(); }

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

    // Removes objects that were marked dead during simulation.
    void processDead();

    // Decides if the level is now won or lost.
    void checkResult();

    // The level's fixed parameters (read-only after construction).
    LevelConfig m_config;

    // Mutable data shared by the game, world, objects, and HUD.
    GameState& m_gameState;

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

};
