#include "Gameplay/World.h"
#include "Gameplay/CollisionHandling.h"
#include "Resources/ResourceManager.h"
#include "Gameplay/Factory.h"
#include "App/AssetConstants.h"
#include "Utilities/Utils.h"
#include <algorithm>
#include <memory>

// Builds the level: creates the vehicle and drone (into m_objects), applies the config.
World::World(const LevelConfig& config, GameState& gameState, int levelNumber)
    : m_config(config)
    , m_gameState(gameState)
    , m_objects()
    , m_enemiesToSpawn(config.enemyCount) {
    m_objects.push_back(Factory::create(GameObjects::CommandCar,
        sf::Vector2f{ WindowConfig::Width / 2.f, EnemyConfig::SpawnY }, m_gameState));
    m_objects.push_back(Factory::create(GameObjects::Drone,
        sf::Vector2f{ WindowConfig::Width / 2.f, DroneConfig::FixedY }, m_gameState));
    m_gameState.startLevel(config, levelNumber);
}

void World::update(float dt) {
    if (m_gameState.getLevelResult() != LevelResult::Running)
        return;

    spawnEnemies(dt);
    for (const auto& object : m_objects)
        object->update(dt);
    handleCollisions();
    collectSpawned();
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
    const GameObject* player = nullptr;
    const GameObject* target = nullptr;
    for (const auto& obj : m_objects) {
        if (!player && obj->isPlayer())
            player = obj.get();
        if (!target && obj->isTarget())
            target = obj.get();
    }
    if (!player || !target)
        return;

    bool fromLeft = (Utils::randomFloat(0.f, 1.f) < 0.5f);
    float x = fromLeft ? 0.f : WindowConfig::Width;
    sf::Vector2f position = { x, EnemyConfig::SpawnY };

    EnemyType type = (Utils::randomFloat(0.f, 1.f) < m_config.tankChance)
        ? EnemyType::Tank : EnemyType::Fast;
    m_objects.push_back(Factory::createEnemy(type, position, m_gameState, *player, *target));
}

void World::collectSpawned() {
    auto spawned = std::vector<std::unique_ptr<GameObject>>{};

    //  Collect every newly spawned child from this frame
    for (auto& obj : m_objects) {
        if (auto child = obj->takeSpawn()) {
            spawned.push_back(std::move(child));
        }
    }

    //  Move the collected children into the main object list
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
    if (isLossConditionMet()) {
        m_gameState.setLevelResult(LevelResult::Lost);
        return;
    }

    if (isWinConditionMet())
        m_gameState.setLevelResult(LevelResult::Won);
}

bool World::isLossConditionMet() const {
    const bool noBombsLeft = m_gameState.getBombsAvailable() == 0 &&
        m_gameState.getActiveBombs() == 0 &&
        m_gameState.getEnemiesRemaining() > 0;
    return m_gameState.getVehicleArmorPercent() <= 0.f ||
        m_gameState.getBatteryPercent() <= 0.f ||
        noBombsLeft;
}

bool World::isWinConditionMet() const {
    return m_gameState.getEnemiesRemaining() == 0;
}

void World::draw(sf::RenderWindow& target) const {
    for (const auto& object : m_objects)
        object->draw(target);
}
