#include "Gameplay/Factory.h"

#include <map>
#include <stdexcept>

#include "Gameplay/GameState.h"
#include "Gameplay/Objects/GameObject.h"
#include "Gameplay/Objects/CommandVehicle.h"
#include "Gameplay/Objects/Drone.h"
#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/Projectiles/Bullet.h"
#include "Gameplay/Enemies/TankEnemy.h"
#include "Gameplay/Enemies/FastEnemy.h"
#include "Resources/ResourceManager.h"
#include "Gameplay/Objects/Explosion.h"

std::unique_ptr<GameObject> Factory::create(GameObjects type, sf::Vector2f position, GameState& gameState) {
    static const std::map<GameObjects, CreatorFunc> creators = {
        { GameObjects::CommandCar, [](sf::Vector2f pos, GameState& state) {
            return std::make_unique<CommandVehicle>(ResourceManager::instance().getImage(GameObjects::CommandCar), pos, state);
        }},
        { GameObjects::Drone, [](sf::Vector2f pos, GameState& state) {
            return std::make_unique<Drone>(ResourceManager::instance().getImage(GameObjects::Drone), pos, state);
        }},
        { GameObjects::Bomb, [](sf::Vector2f pos, GameState& state) {
            return std::make_unique<Bomb>(ResourceManager::instance().getImage(GameObjects::Bomb), pos, state);
        }},
        { GameObjects::Explosion, [](sf::Vector2f pos, GameState& state) {
            return std::make_unique<Explosion>(ResourceManager::instance().getImage(GameObjects::Explosion), pos, state);
        }}

    };

    auto it = creators.find(type);
    if (it == creators.end()) {
        throw std::invalid_argument("Factory::create - Unknown GameObject type!");
    }
    return it->second(position, gameState);
}

std::unique_ptr<GameObject> Factory::createEnemy(EnemyType type, sf::Vector2f position, GameState& gameState, Drone& drone, CommandVehicle& vehicle) {
    static const std::map<EnemyType, EnemyCreatorFunc> enemyCreators = {
        { EnemyType::Tank, [](sf::Vector2f pos, GameState& state, Drone& d, CommandVehicle& v) {
            // Builds a TankEnemy using the GameObjects::Enemy texture
            return std::make_unique<TankEnemy>(
                ResourceManager::instance().getImage(GameObjects::Enemy), pos, state, d, v);
        }},
        { EnemyType::Fast, [](sf::Vector2f pos, GameState& state, Drone& d, CommandVehicle& v) {
            // Builds a FastEnemy using the GameObjects::SmallEnemy texture
            return std::make_unique<FastEnemy>(
                ResourceManager::instance().getImage(GameObjects::SmallEnemy), pos, state, d, v);
        }}
    };

    auto it = enemyCreators.find(type);
    if (it == enemyCreators.end()) {
        throw std::invalid_argument("Factory::createEnemy - Unknown EnemyType!");
    }
    return it->second(position, gameState, drone, vehicle);
}

std::unique_ptr<GameObject> Factory::createBullet(sf::Vector2f position, float velocityX, GameState& gameState) {
    return std::make_unique<Bullet>(
        ResourceManager::instance().getImage(GameObjects::Bullet), position, velocityX, gameState);
}