#pragma once

#include "Gameplay/Objects/GameObject.h"
#include "Gameplay/Objects/Drone.h"
#include "Gameplay/Objects/CommandVehicle.h"
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

    // Steers the player's drone (called by GameScreen from input).
    void moveDroneLeft() { m_drone.moveLeft(); }
    void moveDroneRight() { m_drone.moveRight(); }
    void stopDrone() { m_drone.stop(); }

    // Drops a bomb if the drone still has ammo.
    void dropBomb();

    // Returns whether the level is running, won, or lost.
    LevelResult getResult() const { return m_gameState.getLevelResult(); }

    // Returns the coins earned in this level.
    int getCoinsEarned() const { return m_gameState.getLevelCoins(); }

private:

    // Spawns queued enemies over time.
    void spawnEnemies(float dt);

    // Creates a single enemy at a screen edge (type chosen by the tank chance).
    void spawnEnemy();

    // Updates the drone, the vehicle, and every object in the world.
    void updateObjects(float dt);

    // Adds any objects the world's objects produced this frame (e.g. enemy bullets).
    void collectSpawned();

    // Resolves every overlapping pair of objects via multiple dispatch.
    void handleCollisions();

    // Removes objects that were marked dead during simulation.
    void processDead();

    // Decides if the level is now won or lost.
    void checkResult();

    // The level's fixed parameters (read-only after construction).
    LevelConfig m_config;

    // Mutable data shared by the game, world, objects, and HUD.
    GameState& m_gameState;

    // The player and the protected target. Declared before m_objects so they
    // outlive the enemies, which hold references to them.
    Drone m_drone;
    CommandVehicle m_vehicle;

    // Every dynamic object (enemies, bombs, bullets) as one polymorphic collection.
    std::vector<std::unique_ptr<GameObject>> m_objects;

    // Spawn state that changes during the level.
    int   m_enemiesToSpawn;
    float m_spawnTimer = 0.f;

};