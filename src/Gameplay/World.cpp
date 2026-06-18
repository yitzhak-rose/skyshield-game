#include "Gameplay/World.h"
#include "Resources/ResourceManager.h"
#include "Gameplay/Constants.h"
#include "Gameplay/Enemies/FastEnemy.h"
#include "Gameplay/Enemies/TankEnemy.h"
#include "Utilities/Utils.h"
#include <algorithm>

// Builds the level: creates the drone and vehicle, applies the config.
World::World(const LevelConfig& config, GameState& gameState)
    : m_config(config)
    , m_gameState(gameState)
    , m_drone(ResourceManager::instance().getImage(GameObjects::Drone),
        { WindowConfig::Width / 2.f, DroneConfig::FixedY }, gameState)
    , m_vehicle(ResourceManager::instance().getImage(GameObjects::CommandCar),
        { WindowConfig::Width / 2.f, EnemyConfig::SpawnY }, gameState)
    , m_enemiesToSpawn(config.enemyCount)
{
    m_gameState.startLevel(config);
}

void World::update(float dt) {
    if (m_gameState.getLevelResult() != LevelResult::Running)
        return;

    spawnEnemies(dt);
    updateObjects(dt);
    collectEnemyFire();
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

    bool isTank = (Utils::randomFloat(0.f, 1.f) < m_config.tankChance);
    if (isTank)
        m_enemies.push_back(std::make_unique<TankEnemy>(
            ResourceManager::instance().getImage(GameObjects::Enemy),
            position, m_gameState));
    else
        m_enemies.push_back(std::make_unique<FastEnemy>(
            ResourceManager::instance().getImage(GameObjects::SmallEnemy),
            position, m_gameState));
}

void World::updateObjects(float dt) {
    m_drone.update(dt);
    m_vehicle.update(dt);

    float droneX = m_drone.getPosition().x;
    float vehicleX = m_vehicle.getPosition().x;
    float vehicleHalfWidth = m_vehicle.getBounds().size.x / 2.f;

    for (auto& enemy : m_enemies) {
        enemy->setDroneX(droneX);
        enemy->setVehicleX(vehicleX);
        enemy->setVehicleHalfWidth(vehicleHalfWidth);
        enemy->update(dt);
    }

    for (auto& bomb : m_bombs)
        bomb.update(dt);

    for (auto& bullet : m_bullets)
        bullet.update(dt);
}

void World::collectEnemyFire() {
    float vehicleX = m_vehicle.getPosition().x;

    for (auto& enemy : m_enemies) {
        if (!enemy->isFireRequested())
            continue;

        float enemyX = enemy->getPosition().x;
        float direction = (vehicleX < enemyX) ? -1.f : 1.f;
        m_bullets.emplace_back(ResourceManager::instance().getImage(GameObjects::Bullet),
            enemy->getMuzzlePosition(), direction * BulletConfig::Speed, m_gameState);
        enemy->clearFireRequest();
    }
}

void World::handleCollisions() {
    for (auto& bomb : m_bombs) {
        for (auto& enemy : m_enemies) {
            if (bomb.getBounds().findIntersection(enemy->getBounds())) {
                bomb.destroy();
                enemy->onHit();
                break;
            }
        }
    }

    for (auto& bullet : m_bullets) {
        if (bullet.getBounds().findIntersection(m_vehicle.getBounds())) {
            bullet.destroy();
            m_vehicle.takeDamage();
        }
    }
}

void World::processDead() {
    std::erase_if(m_enemies, [](const std::unique_ptr<Enemy>& enemy) {
        return enemy->isDead();
        });

    std::erase_if(m_bombs, [](const Bomb& bomb) {
        return bomb.isDead();
        });

    std::erase_if(m_bullets, [](const Bullet& bullet) {
        return bullet.isDead();
        });
}

void World::checkResult() {
    if (m_vehicle.isDead() ||
        m_gameState.getBatteryPercent() <= 0.f ||
        (m_gameState.getBombsAvailable() == 0 && !m_enemies.empty() && m_bombs.empty())) {
        m_gameState.setLevelResult(LevelResult::Lost);
        return;
    }

    if (m_enemies.empty() && m_enemiesToSpawn == 0)
        m_gameState.setLevelResult(LevelResult::Won);
}

void World::draw(sf::RenderWindow& window) const {
    m_vehicle.draw(window);

    for (const auto& enemy : m_enemies)
        enemy->draw(window);

    for (const auto& bullet : m_bullets)
        bullet.draw(window);

    for (const auto& bomb : m_bombs)
        bomb.draw(window);

    m_drone.draw(window);
}

void World::dropBomb() {
    if (!m_drone.consumeAmmo())
        return;

    sf::Vector2f position = m_drone.getPosition();
    m_bombs.emplace_back(
        ResourceManager::instance().getImage(GameObjects::Bomb),
        position,
        m_gameState);
}
