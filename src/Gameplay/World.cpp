#include "Gameplay/World.h"
#include "Gameplay/Enemies/EnemyFactory.h"
#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/CollisionHandling.h"
#include "Resources/ResourceManager.h"
#include "Gameplay/Constants.h"
#include "Utilities/Utils.h"
#include <algorithm>
#include <memory>

// Builds the level: creates the drone and vehicle, applies the config.
World::World(const LevelConfig& config, GameState& gameState, int levelNumber)
    : m_config(config)
    , m_gameState(gameState)
    , m_drone(ResourceManager::instance().getImage(GameObjects::Drone),
        { WindowConfig::Width / 2.f, DroneConfig::FixedY }, gameState)
    , m_vehicle(ResourceManager::instance().getImage(GameObjects::CommandCar),
        { WindowConfig::Width / 2.f, EnemyConfig::SpawnY }, gameState)
    , m_enemiesToSpawn(config.enemyCount)
{
    m_gameState.startLevel(config, levelNumber);
}

void World::update(float dt) {
    if (m_gameState.getLevelResult() != LevelResult::Running)
        return;

    spawnEnemies(dt);
    updateObjects(dt);
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
    m_objects.push_back(EnemyFactory::create(type, position, m_gameState, m_drone, m_vehicle));
}

void World::updateObjects(float dt) {
    // The drone and the vehicle update first, so enemies read their current state.
    m_drone.update(dt);
    m_vehicle.update(dt);

    for (auto& object : m_objects)
        object->update(dt);
}

void World::collectSpawned() {
    // Gather first, then append: appending while iterating would invalidate the loop.
    std::vector<std::unique_ptr<GameObject>> spawned;
    for (auto& object : m_objects)
        if (auto child = object->takeSpawn())
            spawned.push_back(std::move(child));

    for (auto& child : spawned)
        m_objects.push_back(std::move(child));
}

void World::handleCollisions() {
    // The vehicle is the only collidable that is not in m_objects, so it is
    // tested explicitly against each object. Every object is also tested
    // against every later object. Dead objects are skipped, which keeps a
    // single bomb from striking more than one enemy in the same frame.
    for (std::size_t i = 0; i < m_objects.size(); ++i) {
        GameObject& a = *m_objects[i];
        if (a.isDead())
            continue;

        if (a.getBounds().findIntersection(m_vehicle.getBounds()))
            processCollision(a, m_vehicle);

        for (std::size_t j = i + 1; j < m_objects.size(); ++j) {
            if (a.isDead())
                break;

            GameObject& b = *m_objects[j];
            if (b.isDead())
                continue;

            if (a.getBounds().findIntersection(b.getBounds()))
                processCollision(a, b);
        }
    }
}

void World::processDead() {
    std::erase_if(m_objects, [](const std::unique_ptr<GameObject>& object) {
        return object->isDead();
        });
}

void World::checkResult() {
    // Lost: the vehicle is destroyed, the battery is empty, or the level is
    // unwinnable (no ammo, no bomb still falling, and enemies still remain).
    if (m_vehicle.isDead() ||
        m_gameState.getBatteryPercent() <= 0.f ||
        (m_gameState.getBombsAvailable() == 0 &&
            m_gameState.getActiveBombs() == 0 &&
            m_gameState.getEnemiesRemaining() > 0)) {
        m_gameState.setLevelResult(LevelResult::Lost);
        return;
    }

    // Won: every enemy has been defeated.
    if (m_gameState.getEnemiesRemaining() == 0)
        m_gameState.setLevelResult(LevelResult::Won);
}

void World::draw(sf::RenderTarget& target) const {
    // The vehicle sits at the back; the drone draws on top of everything.
    m_vehicle.draw(target);

    for (const auto& object : m_objects)
        object->draw(target);

    m_drone.draw(target);
}

void World::dropBomb() {
    if (!m_drone.consumeAmmo())
        return;

    m_objects.push_back(std::make_unique<Bomb>(
        ResourceManager::instance().getImage(GameObjects::Bomb),
        m_drone.getPosition(),
        m_gameState));
}