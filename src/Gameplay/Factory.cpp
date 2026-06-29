#include "Gameplay/Factory.h"
#include "App/State/GameState.h"
#include "Gameplay/Objects/GameObject.h"
#include "Gameplay/Objects/CommandVehicle.h"
#include "Gameplay/Objects/Drone.h"
#include "Gameplay/Objects/Explosion.h"
#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/Projectiles/Bullet.h"
#include "Gameplay/Enemies/TankEnemy.h"
#include "Gameplay/Enemies/FastEnemy.h"
#include "Resources/ResourceManager.h"
#include <stdexcept>

Factory& Factory::instance() {
    static Factory _instance;
    return _instance;
}

Factory::Factory()
    : m_creators{
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
    }
    , m_enemyCreators{
        { EnemyType::Tank, [](sf::Vector2f pos, GameState& state, const GameObject& player, const GameObject& target) {
            return std::make_unique<TankEnemy>(
                ResourceManager::instance().getImage(GameObjects::Enemy), pos, state, player, target);
        }},
        { EnemyType::Fast, [](sf::Vector2f pos, GameState& state, const GameObject& player, const GameObject& target) {
            return std::make_unique<FastEnemy>(
                ResourceManager::instance().getImage(GameObjects::SmallEnemy), pos, state, player, target);
        }}
    }
{
}

std::unique_ptr<GameObject> Factory::create(GameObjects type, sf::Vector2f position, GameState& gameState) {
    auto it = m_creators.find(type);
    if (it == m_creators.end()) {
        throw std::invalid_argument("Factory::create - Unknown GameObject type!");
    }
    return it->second(position, gameState);
}

std::unique_ptr<GameObject> Factory::createEnemy(EnemyType type, sf::Vector2f position, GameState& gameState, const GameObject& player, const GameObject& target) {
    auto it = m_enemyCreators.find(type);
    if (it == m_enemyCreators.end()) {
        throw std::invalid_argument("Factory::createEnemy - Unknown EnemyType!");
    }
    return it->second(position, gameState, player, target);
}

std::unique_ptr<GameObject> Factory::createBullet(sf::Vector2f position, float velocityX, GameState& gameState) {
    return std::make_unique<Bullet>(
        ResourceManager::instance().getImage(GameObjects::Bullet), position, velocityX, gameState);
}