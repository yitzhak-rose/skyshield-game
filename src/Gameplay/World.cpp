#include "Gameplay/World.h"
#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/CollisionHandling.h"
#include "Resources/ResourceManager.h"
#include "Gameplay/Constants.h"
#include "Utilities/Utils.h"
#include <algorithm>
#include <memory>
#include "Gameplay/Factory.h" 

// Builds the level: creates the vehicle and drone (into m_objects), applies the config.
World::World(const LevelConfig& config, GameState& gameState, int levelNumber)
    : m_config(config)
    , m_gameState(gameState)
    , m_objects()
    , m_vehicle(Factory::createAndAdd<CommandVehicle>(GameObjects::CommandCar,
        sf::Vector2f{ WindowConfig::Width / 2.f, EnemyConfig::SpawnY }, m_gameState, m_objects))
    , m_drone(Factory::createAndAdd<Drone>(GameObjects::Drone, 
        sf::Vector2f{ WindowConfig::Width / 2.f, DroneConfig::FixedY }, m_gameState, m_objects))
    , m_enemiesToSpawn(config.enemyCount)
{
    m_gameState.startLevel(config, levelNumber);
}

void World::update(float dt) {
    if (m_gameState.getLevelResult() != LevelResult::Running)
        return;

    spawnEnemies(dt);
    for (const auto& object : m_objects)
        object->update(dt);
    collectSpawned();
    handleCollisions();
    processDead();
    checkResult();
}

void World::spawnEnemies(float dt) {
    if (m_enemiesToSpawn == 0)
        return;

    m_spawnTimer += dt;
    if (m_spawnTimer >= m_config.spawnInterval) {
        spawnEnemy();
        m_enemiesToSpawn--;
        m_spawnTimer = 0.f;
    }
}

void World::spawnEnemy() {
    bool fromLeft = (Utils::randomFloat(0.f, 1.f) < 0.5f);
    float x = fromLeft ? 0.f : WindowConfig::Width;
    sf::Vector2f position = { x, EnemyConfig::SpawnY };

    EnemyType type = (Utils::randomFloat(0.f, 1.f) < m_config.tankChance)
        ? EnemyType::Tank : EnemyType::Fast;
    m_objects.push_back(Factory::createEnemy(type, position, m_gameState, m_drone, m_vehicle));
}

void World::collectSpawned() {
    auto spawned = std::vector<std::unique_ptr<GameObject>>{};

    // 1. Collect every newly spawned child from this frame
    for (auto& obj : m_objects) {
        if (auto child = obj->takeSpawn()) {
            spawned.push_back(std::move(child));
        }
    }

    // 2. Move the collected children into the main object list
    m_objects.insert(
        m_objects.end(),
        std::make_move_iterator(spawned.begin()),
        std::make_move_iterator(spawned.end())
    );
}

void World::handleCollisions() {
    for (std::size_t i = 0; i < m_objects.size(); i++)
        for (std::size_t j = i + 1; j < m_objects.size(); j++) {
            if (!m_objects[i]->isDead() && !m_objects[j]->isDead() &&
                m_objects[i]->getBounds().findIntersection(m_objects[j]->getBounds()))
                processCollision(*m_objects[i], *m_objects[j]);
        }
}

void World::processDead() {
    std::erase_if(m_objects, [](const std::unique_ptr<GameObject>& object) {
        return object->isDead();
        });
}

void World::checkResult() {
    if (m_gameState.getVehicleArmorPercent() <= 0.f ||
        m_gameState.getBatteryPercent() <= 0.f ||
        (m_gameState.getBombsAvailable() == 0 &&
            m_gameState.getActiveBombs() == 0 &&
            m_gameState.getEnemiesRemaining() > 0)) {
        m_gameState.setLevelResult(LevelResult::Lost);
        return;
    }

    if (m_gameState.getEnemiesRemaining() == 0)
        m_gameState.setLevelResult(LevelResult::Won);
}

void World::draw(sf::RenderTarget& target) const {
    for (const auto& object : m_objects)
        object->draw(target);
}

void World::dropBomb() {
    //  Check there's ammo left (and if so, take one bomb off the count)
    if (!m_drone.consumeAmmo())
        return;

    //  Build the bomb through the factory and push it straight into the object list
    m_objects.push_back(Factory::create(
        GameObjects::Bomb,
        m_drone.getPosition(),
        m_gameState
    ));
}