#pragma once

#include <SFML/Graphics/Color.hpp>

// All game-wide tunable values, grouped by domain.

namespace VehicleConfig {
    inline constexpr float StartArmor = 100.f; // armor starts full (percent)
    inline constexpr float DamagePerHit = 1.f;  // armor lost per bullet hit
    inline constexpr int   FrameCount = 6;     // beacon + smoke frames in commandCar.png
    inline constexpr float FrameDuration = 0.15f; // seconds each frame is shown
    inline constexpr float Scale = 1.8f;       // larger than life — it's the centerpiece
    inline constexpr int   NormalFrames = 4;   // frames 0..3 loop while healthy (beacon)
    inline constexpr float CriticalArmor = 25.f; // below this, show the burning frames 4..5
    inline constexpr float ShakeDuration = 0.18f;  // seconds the vehicle shakes after a hit
    inline constexpr float ShakeMagnitude = 6.f;   // peak horizontal offset (pixels) at hit
    inline constexpr float ShakeSpeed = 60.f;      // how fast the shake oscillates
}

namespace WindowConfig {
    inline constexpr float Width = 1800.f; // window width in pixels
    inline constexpr float Height = 720.f;  // window height in pixels
}

namespace BombConfig {
    inline constexpr float FallSpeed = 400.f;     // downward pixels per second
    inline constexpr int   FrameCount = 4;        // wobble frames in bomb.png
    inline constexpr float FrameDuration = 0.1f;  // seconds each frame is shown
    inline constexpr float Scale = 1.f;           // on-screen size multiplier
}

namespace BulletConfig {
    inline constexpr float Speed = 250.f; // horizontal pixels per second
    inline constexpr float Scale = 1.f;   // bullet.png is a single cropped frame
}

namespace DroneConfig {
    inline constexpr float Speed = 300.f; // horizontal pixels per second
    inline constexpr float FixedY = 150.f;  // locked vertical position
    inline constexpr float StartBattery = 100.f; // battery starts full (percent)
    inline constexpr int   FrameCount = 4;     // rotor frames in drone.png
    inline constexpr float FrameDuration = 0.08f; // seconds each frame is shown
    inline constexpr float Scale = 2.f;        // on-screen size multiplier
}

namespace BatteryConfig {
    inline constexpr float FullPercent = 100.f;
    inline constexpr int FrameCount = 5;
    inline constexpr float FrameDuration = 1.f;
    inline constexpr float Scale = 1.f;
}

namespace MenuBarConfig {
    inline constexpr unsigned int TextSize = 32;
    inline constexpr unsigned int TitleTextSize = 42;
    inline constexpr float SidePadding = 28.f;
    inline constexpr float IconSize = 46.f;
    inline constexpr float BatteryHeight = 46.f;
    inline constexpr float BombTextGap = 14.f;
    inline const sf::Color BackgroundColor(16, 20, 28, 220);
    inline const sf::Color TitleColor(244, 232, 178);
    inline constexpr float Height = 76.f; // menu bar height in pixels
    inline constexpr float CoinIconX = 260.f; // coin counter slot, just right of the bomb counter
}

namespace EnemyConfig {
    inline constexpr float SpawnY = 575.f; // ground level (all enemies on this line)
    inline constexpr float StopRangeMin = 150.f; // min stopping distance from vehicle
    inline constexpr float StopRangeMax = 230.f; // max stopping distance from vehicle
    inline constexpr float PanicMultiplier = 2.f;   // flee speed = base speed × this
    inline constexpr float TelegraphTime = 1.5f;  // seconds between shots (per design doc)
    inline constexpr float UTurnTrigger = 5.f;  // distance from a wall that starts a U-turn
    inline constexpr float UTurnDistance = 80.f; // distance to travel before obeying the drone again
    inline constexpr float VehicleGap = 60.f; // how close to the vehicle an enemy may get while fleeing
    inline constexpr float PanicRange = 100.f; // drone within this range triggers panic
    inline constexpr float PanicExitRange = 220.f; // stay panicking until the drone is past this
}

namespace FastEnemyConfig {
    inline constexpr float Speed = 120.f; // fast but fragile
    inline constexpr int   Reward = 10;    // coins when killed
    inline constexpr int   FrameCount = 4;     // driving frames in smallEnemy.png
    inline constexpr float FrameDuration = 0.12f; // seconds each frame is shown
    inline constexpr float Scale = 1.f;        // on-screen size multiplier
    inline constexpr float MuzzleX = 12.f;     // gun tip: slightly forward of center
    inline constexpr float MuzzleY = -38.f;    // gun tip on the roof, raised so the flash centers on the bore
}

namespace TankEnemyConfig {
    inline constexpr float Speed = 60.f;  // slow but armored
    inline constexpr int   Reward = 20;    // coins when killed
    inline constexpr int   FrameCount = 5;     // firing frames in enemy.png
    inline constexpr float FrameDuration = 0.3f;  // 5 frames × 0.3s = 1.5s = TelegraphTime (frames track the fire timer)
    inline constexpr float Scale = 0.6f;       // sheet frame is large; shrink to fit
    inline constexpr float MuzzleX = 101.f;    // cannon tip: well forward (long barrel)
    inline constexpr float MuzzleY = -17.f;    // cannon tip height, raised so the flash centers on the bore
}

namespace CoinConfig {
    inline constexpr int   FrameCount = 7;     // spinning frames in coin.png
    inline constexpr float FrameDuration = 0.08f; // seconds each spin frame is shown
    inline constexpr float Scale = 1.f;   // base on-screen size of the icon
    inline constexpr float PopDuration = 0.2f;  // seconds the count "pops" after it grows
    inline constexpr float PopMagnitude = 0.4f;  // extra scale at the peak of the pop (0.4 = +40%)
}