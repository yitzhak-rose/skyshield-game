#pragma once
#include "Gameplay/Enemies/Enemy.h"
#include "Enums/EnemyType.h"
#include <memory>

class GameState;
class Drone;
class CommandVehicle;

/// @brief Builds concrete Enemy objects from an EnemyType.
///        Centralizes the class + texture choice, so World only decides
///        which type to spawn, not how to construct it.
class EnemyFactory {
public:
    /// @brief Creates a concrete enemy of the given type at a position,
    ///        wired to the live drone and vehicle the enemy reacts to.
    static std::unique_ptr<Enemy> create(EnemyType type, sf::Vector2f position,
        GameState& gameState, const Drone& drone, const CommandVehicle& vehicle);
};