#include "Gameplay/Enemies/EnemyFactory.h"
#include "Gameplay/Enemies/FastEnemy.h"
#include "Gameplay/Enemies/TankEnemy.h"
#include "Resources/ResourceManager.h"
#include <stdexcept>

// Maps each EnemyType to its concrete class and its sprite-sheet texture.
std::unique_ptr<Enemy> EnemyFactory::create(EnemyType type, sf::Vector2f position,
    GameState& gameState, const Drone& drone, const CommandVehicle& vehicle) {
    switch (type) {
    case EnemyType::Fast:
        return std::make_unique<FastEnemy>(
            ResourceManager::instance().getImage(GameObjects::SmallEnemy),
            position, gameState, drone, vehicle);
    case EnemyType::Tank:
        return std::make_unique<TankEnemy>(
            ResourceManager::instance().getImage(GameObjects::Enemy),
            position, gameState, drone, vehicle);
    }
    throw std::runtime_error("EnemyFactory: unknown EnemyType");
}