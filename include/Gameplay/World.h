#pragma once
#include "Gameplay/Objects/GameObject.h"
#include "Gameplay/GameState.h"
#include "Gameplay/LevelConfig.h"
#include <vector>
#include <memory>

/// @brief Owns and drives all gameplay objects for a single level: spawning enemies,
///        updating and drawing the unified object list, resolving collisions, and
///        tracking whether the level has been won or lost.
class World {
public:
    /// @brief Builds the level: creates the vehicle and drone, and applies the level config.
    World(const LevelConfig& config, GameState& gameState, int levelNumber);

    /// @brief Advances spawning, all objects, collisions, and the win/loss check by one frame.
    void update(float dt);

    /// @brief Draws every live object in the world.
    void draw(sf::RenderWindow& target) const;

    /// @brief Returns whether the level is still running, won, or lost.
    LevelResult getResult() const { return m_gameState.getLevelResult(); }

    /// @brief Returns the coins collected so far in the current level.
    int getCoinsEarned() const { return m_gameState.getLevelCoins(); }

private:
    /// @brief Spawns a new enemy once the spawn interval has elapsed, while any remain.
    void spawnEnemies(float dt);

    /// @brief Creates a single enemy at a random screen edge.
    void spawnEnemy();

    /// @brief Moves objects spawned by other objects this frame into the main object list.
    void collectSpawned();

    /// @brief Checks every pair of live objects for overlap and resolves any collisions.
    void handleCollisions();

    /// @brief Removes all objects marked dead this frame.
    void processDead();

    /// @brief Updates the level result based on vehicle health, battery, and enemies remaining.
    void checkResult();

    /// @brief Returns true when the level can no longer be completed.
    bool isLossConditionMet() const;

    /// @brief Returns true when all enemies have been defeated.
    bool isWinConditionMet() const;

    LevelConfig m_config;
    GameState& m_gameState;

    std::vector<std::unique_ptr<GameObject>> m_objects;

    int   m_enemiesToSpawn;
    float m_spawnTimer = 0.f;
};
