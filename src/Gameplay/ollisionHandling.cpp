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

using HitFunc = void(*)(GameObject&, GameObject&);
using Key = std::pair<std::type_index, std::type_index>;

void processCollision(GameObject& o1, GameObject& o2) {
    static const std::map<Key, HitFunc> map = {
        {{typeid(Bomb), typeid(FastEnemy)}, [](GameObject& b, GameObject& e) { b.destroy(); static_cast<Enemy&>(e).onHit(); }},
        {{typeid(FastEnemy), typeid(Bomb)}, [](GameObject& e, GameObject& b) { b.destroy(); static_cast<Enemy&>(e).onHit(); }},
        {{typeid(Bomb), typeid(TankEnemy)}, [](GameObject& b, GameObject& e) { b.destroy(); static_cast<Enemy&>(e).onHit(); }},
        {{typeid(TankEnemy), typeid(Bomb)}, [](GameObject& e, GameObject& b) { b.destroy(); static_cast<Enemy&>(e).onHit(); }},
        {{typeid(Bullet), typeid(CommandVehicle)}, [](GameObject& b, GameObject& v) { b.destroy(); static_cast<CommandVehicle&>(v).takeDamage(); }},
        {{typeid(CommandVehicle), typeid(Bullet)}, [](GameObject& v, GameObject& b) { b.destroy(); static_cast<CommandVehicle&>(v).takeDamage(); }}
    };

    auto it = map.find({ typeid(o1), typeid(o2) });
    if (it != map.end()) {
        it->second(o1, o2);
    }
}