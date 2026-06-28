#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Enums/GameObjects.h"
#include "Enums/EnemyType.h"
#include "Gameplay/Objects/GameObject.h"
class GameState;
class Drone;
class CommandVehicle;

/// @brief Static factory that builds GameObjects (and enemies, which need extra targets)
///        from an enum tag, looking up the right texture via ResourceManager.
class Factory {
public:
    using CreatorFunc = std::function<std::unique_ptr<GameObject>(sf::Vector2f, GameState&)>;

    /// @brief Builds a regular GameObject of the given type at a position.
    static std::unique_ptr<GameObject> create(GameObjects type, sf::Vector2f position, GameState& gameState);

    using EnemyCreatorFunc = std::function<std::unique_ptr<GameObject>(sf::Vector2f, GameState&, Drone&, CommandVehicle&)>;

    /// @brief Builds an enemy of the given type, wiring it to the drone and vehicle it targets.
    static std::unique_ptr<GameObject> createEnemy(EnemyType type, sf::Vector2f position, GameState& gameState, Drone& drone, CommandVehicle& vehicle);

    /// @brief Builds a bullet at a position with a given horizontal velocity (fired by an enemy).
    static std::unique_ptr<GameObject> createBullet(sf::Vector2f position, float velocityX, GameState& gameState);

    /// @brief Creates a regular GameObject, pushes it into the given container, and returns a typed reference to it.
    template <typename T>
    static T& createAndAdd(GameObjects type, sf::Vector2f position, GameState& gameState, std::vector<std::unique_ptr<GameObject>>& container) {
        auto obj = create(type, position, gameState);
        T& ref = static_cast<T&>(*obj);
        container.push_back(std::move(obj));
        return ref;
    }
};