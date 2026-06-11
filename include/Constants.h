#pragma once

// All game-wide tunable values, grouped by domain.

namespace VehicleConfig {
    inline constexpr float StartArmor = 100.f; // armor starts full (percent)
    inline constexpr float DamagePerHit = 10.f;  // armor lost per bullet hit
}

namespace WindowConfig {
    inline constexpr float Width = 1280.f; // window width in pixels
    inline constexpr float Height = 720.f;  // window height in pixels
}

namespace BombConfig {
    inline constexpr float FallSpeed = 400.f; // downward pixels per second
}

namespace BulletConfig {
    inline constexpr float Speed = 250.f; // horizontal pixels per second
}

namespace DroneConfig {
    inline constexpr float Speed = 300.f; // horizontal pixels per second
    inline constexpr float FixedY = 80.f;  // locked vertical position
    inline constexpr float StartBattery = 100.f; // battery starts full (percent)
}

namespace EnemyConfig {
    inline constexpr float SpawnY = 600.f; // ground level (all enemies on this line)
    inline constexpr float StopRangeMin = 150.f; // min stopping distance from vehicle
    inline constexpr float StopRangeMax = 230.f; // max stopping distance from vehicle
    inline constexpr float PanicRange = 100.f; // drone within this range triggers panic
    inline constexpr float PanicMultiplier = 2.f;   // flee speed = base speed × this
    inline constexpr float TelegraphTime = 0.4f;  // red "charging" time before firing
    inline constexpr float UTurnTrigger = 5.f;  // distance from a wall that starts a U-turn
    inline constexpr float UTurnDistance = 80.f; // distance to travel before obeying the drone again
    inline constexpr float VehicleGap = 60.f; // how close to the vehicle an enemy may get while fleeing
}

namespace FastEnemyConfig {
    inline constexpr float Speed = 120.f; // fast but fragile
    inline constexpr int   Reward = 10;    // coins when killed
}

namespace TankEnemyConfig {
    inline constexpr float Speed = 60.f;  // slow but armored
    inline constexpr int   Reward = 20;    // coins when killed
}