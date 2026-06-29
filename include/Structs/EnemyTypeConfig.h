#pragma once

#include "Structs/AnimatedSpriteConfig.h"
#include "Structs/MuzzleOffset.h"

/// @brief Gameplay and animation tuning for one enemy type.
struct EnemyTypeConfig {
    float speed = 0.f;
    int reward = 0;
    AnimatedSpriteConfig sprite;
    MuzzleOffset muzzle;
};
