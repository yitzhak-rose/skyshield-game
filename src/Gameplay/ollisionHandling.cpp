#include "Gameplay/CollisionHandling.h"
#include "Gameplay/Objects/GameObject.h"
#include "Gameplay/Objects/CommandVehicle.h"
#include "Gameplay/Enemies/Enemy.h"
#include "Gameplay/Enemies/FastEnemy.h"
#include "Gameplay/Enemies/TankEnemy.h"
#include "Gameplay/Projectiles/Bomb.h"
#include "Gameplay/Projectiles/Bullet.h"

#include <map>
#include <typeindex>
#include <typeinfo>

namespace // anonymous namespace — the standard way to make these functions "static"
{
    // primary collision-processing functions
    // The map keys guarantee the dynamic types, so the casts inside are safe.
    void bombEnemy(GameObject& bomb, GameObject& enemy)
    {
        bomb.destroy();                          // destroy() lives on GameObject — no cast needed
        static_cast<Enemy&>(enemy).onHit();      // FastEnemy / TankEnemy both derive from Enemy
    }

    void bulletVehicle(GameObject& bullet, GameObject& vehicle)
    {
        bullet.destroy();
        static_cast<CommandVehicle&>(vehicle).takeDamage();
    }

    // secondary collision-processing functions that just implement symmetry:
    // swap the parameters and call a primary function
    void enemyBomb(GameObject& enemy, GameObject& bomb) { bombEnemy(bomb, enemy); }
    void vehicleBullet(GameObject& vehicle, GameObject& bullet) { bulletVehicle(bullet, vehicle); }

    using HitFunctionPtr = void (*)(GameObject&, GameObject&);
    using Key = std::pair<std::type_index, std::type_index>;
    // std::unordered_map would need a hash for the pair; std::map only needs ordering.
    using HitMap = std::map<Key, HitFunctionPtr>;

    HitMap initializeCollisionMap()
    {
        HitMap phm;
        // a dropped bomb destroys itself and hits whichever enemy it lands on
        phm[Key(typeid(Bomb), typeid(FastEnemy))] = &bombEnemy;
        phm[Key(typeid(FastEnemy), typeid(Bomb))] = &enemyBomb;
        phm[Key(typeid(Bomb), typeid(TankEnemy))] = &bombEnemy;
        phm[Key(typeid(TankEnemy), typeid(Bomb))] = &enemyBomb;
        // an enemy bullet damages the command vehicle
        phm[Key(typeid(Bullet), typeid(CommandVehicle))] = &bulletVehicle;
        phm[Key(typeid(CommandVehicle), typeid(Bullet))] = &vehicleBullet;
        // every other pair is intentionally absent: those objects do not interact.
        return phm;
    }

    HitFunctionPtr lookup(const std::type_index& class1, const std::type_index& class2)
    {
        static HitMap collisionMap = initializeCollisionMap();
        auto mapEntry = collisionMap.find(std::make_pair(class1, class2));
        if (mapEntry == collisionMap.end())
            return nullptr;
        return mapEntry->second;
    }
} // end namespace

void processCollision(GameObject& object1, GameObject& object2)
{
    auto phf = lookup(typeid(object1), typeid(object2));
    if (phf)
        phf(object1, object2);
    // No handler for this pair => the two objects simply pass through each other.
}