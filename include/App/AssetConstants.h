#pragma once

#include "App/Constants.h"
#include "Structs/AnimatedSpriteConfig.h"
#include "Structs/EnemyTypeConfig.h"

namespace VehicleConfig {
    inline constexpr float StartArmor = DefaultConfig::FullPercent;
    inline constexpr float DamagePerHit = 1.f;
    inline constexpr AnimatedSpriteConfig Sprite{ 6, 0.15f, 1.8f };
    inline constexpr int NormalFrames = 4;
    inline constexpr float CriticalArmor = 25.f;
    inline constexpr float ShakeDuration = 0.18f;
    inline constexpr float ShakeMagnitude = 6.f;
    inline constexpr float ShakeSpeed = 60.f;
}

namespace BombConfig {
    inline constexpr float FallSpeed = 400.f;
    inline constexpr AnimatedSpriteConfig Sprite{ 4, 0.1f };
}

namespace BulletConfig {
    inline constexpr float Speed = 250.f;
}

namespace DroneConfig {
    inline constexpr float Speed = 300.f;
    inline constexpr float FixedY = 150.f;
    inline constexpr float StartBattery = DefaultConfig::FullPercent;
    inline constexpr AnimatedSpriteConfig Sprite{ 4, 0.08f };
    inline constexpr float DisplayWidth = 156.f;
    inline constexpr float DisplayHeight = 120.f;
}

namespace BatteryConfig {
    inline constexpr float FullPercent = DefaultConfig::FullPercent;
    inline constexpr int FrameCount = 5;
}

namespace EnemyConfig {
    inline constexpr float SpawnY = 575.f;
    inline constexpr float StopRangeMin = 150.f;
    inline constexpr float StopRangeMax = 330.f;
    inline constexpr float PanicMultiplier = 2.f;
    inline constexpr float TelegraphTime = 1.5f;
    inline constexpr float UTurnTrigger = 5.f;
    inline constexpr float UTurnDistance = 80.f;
    inline constexpr float VehicleGap = 60.f;
    inline constexpr float PanicRange = 100.f;
    inline constexpr float PanicExitRange = 220.f;
}

namespace FastEnemyConfig {
    inline constexpr EnemyTypeConfig Stats{
        120.f, 10, { 4, 0.12f, DefaultConfig::SpriteScale }, { 12.f, -38.f }
    };
}

namespace TankEnemyConfig {
    inline constexpr EnemyTypeConfig Stats{
        80.f, 20, { 5, 0.3f, 0.6f }, { 101.f, -17.f }
    };
}

namespace CoinConfig {
    inline constexpr AnimatedSpriteConfig Sprite{ 7, 0.08f };
    inline constexpr float PopDuration = 0.2f;
    inline constexpr float PopMagnitude = 0.4f;
}

namespace ExplosionConfig {
    inline constexpr int FrameCount = 8;
    inline constexpr float FrameDuration = 0.08f;
    inline constexpr float Scale = 1.2f;
}
