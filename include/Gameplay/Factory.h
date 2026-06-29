#pragma once

#include <functional>
#include <map>
#include <memory>
#include <SFML/System/Vector2.hpp>

#include "Enums/GameObjects.h"
#include "Enums/EnemyType.h"
#include "Gameplay/Objects/GameObject.h"
class GameState;

/// @brief Singleton factory that builds GameObjects (and enemies, which need extra targets)
///        from an enum tag, looking up the right texture via ResourceManager.
class Factory {
public:
    using CreatorFunc = std::function<std::unique_ptr<GameObject>(sf::Vector2f, GameState&)>;
    using EnemyCreatorFunc = std::function<std::unique_ptr<GameObject>(sf::Vector2f, GameState&, const GameObject&, const GameObject&)>;

    /// @brief Gets the singleton instance of the Factory.
    static Factory& instance();

    /// @brief Builds a regular GameObject of the given type at a position.
    std::unique_ptr<GameObject> create(GameObjects type, sf::Vector2f position, GameState& gameState);

    /// @brief Builds an enemy of the given type, wiring it to the drone and vehicle it targets.
    std::unique_ptr<GameObject> createEnemy(EnemyType type, sf::Vector2f position, GameState& gameState, const GameObject& player, const GameObject& target);

    /// @brief Builds a bullet at a position with a given horizontal velocity (fired by an enemy).
    std::unique_ptr<GameObject> createBullet(sf::Vector2f position, float velocityX, GameState& gameState);

private:
    /// @brief Private constructor initializes the lookup tables once for the lifetime of the program.
    Factory();
    ~Factory() = default;
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    /// @brief Lookup table mapping each GameObject type to its construction lambda.
    std::map<GameObjects, CreatorFunc> m_creators;

    /// @brief Lookup table mapping each EnemyType to its construction lambda.
    std::map<EnemyType, EnemyCreatorFunc> m_enemyCreators;
};