#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstddef>

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
    inline constexpr unsigned int LevelTextSize = 28;
    inline constexpr float SidePadding = 28.f;
    inline constexpr float IconSize = 46.f;
    inline constexpr float BatteryHeight = 46.f;
    inline constexpr float BombTextGap = 14.f;
    inline constexpr float LevelBatteryGap = 24.f;
    inline constexpr float VehicleArmorBarX = 390.f;
    inline constexpr float VehicleArmorBarWidth = 260.f;
    inline constexpr float VehicleArmorBarHeight = 18.f;
    inline constexpr float VehicleArmorBarOutlineThickness = 2.f;
    inline constexpr unsigned int VehicleArmorTextSize = 18;
    inline const sf::Color BackgroundColor(16, 20, 28, 220);
    inline const sf::Color TitleColor(244, 232, 178);
    inline const sf::Color LevelTextColor(235, 238, 245);
    inline const sf::Color VehicleArmorTrackColor(42, 48, 58, 230);
    inline const sf::Color VehicleArmorOutlineColor(235, 238, 245, 180);
    inline const sf::Color VehicleArmorHealthyColor(67, 176, 113);
    inline const sf::Color VehicleArmorWarningColor(235, 168, 66);
    inline const sf::Color VehicleArmorCriticalColor(211, 77, 83);
    inline constexpr float Height = 76.f; // menu bar height in pixels
    inline constexpr float CoinIconX = 260.f; // coin counter slot, just right of the bomb counter
}

namespace OverlayViewConfig {
    inline constexpr std::size_t MaxActions = 3;
    inline constexpr float PanelWidth = 660.f;
    inline constexpr float PanelHeight = 360.f;
    inline constexpr float PanelOutlineThickness = 3.f;
    inline constexpr float ButtonWidth = 220.f;
    inline constexpr float ButtonHeight = 64.f;
    inline constexpr float ButtonGap = 30.f;
    inline constexpr float ButtonTopOffset = 82.f;
    inline constexpr float TitleTopOffset = -112.f;
    inline constexpr float BodyTopOffset = -35.f;
    inline constexpr float BodyLineSpacing = 38.f;
    inline constexpr float ButtonOutlineThickness = 2.f;
    inline constexpr float SelectedButtonOutlineThickness = 4.f;
    inline constexpr int SelectedColorBoost = 24;
    inline constexpr unsigned int TitleTextSize = 48;
    inline constexpr unsigned int BodyTextSize = 28;
    inline constexpr unsigned int ButtonTextSize = 30;
    inline const sf::Color ScrimColor(0, 0, 0, 135);
    inline const sf::Color PanelColor(16, 20, 28, 235);
    inline const sf::Color PanelOutlineColor(244, 232, 178, 210);
    inline const sf::Color TitleColor(244, 232, 178);
    inline const sf::Color BodyColor(235, 238, 245);
    inline const sf::Color ButtonTextColor(255, 255, 255);
    inline const sf::Color ButtonOutlineColor(255, 255, 255, 220);
    inline const sf::Color DefaultButtonColor(58, 68, 86);
    inline const sf::Color DisabledButtonColor(74, 78, 86);
    inline const sf::Color PrimaryButtonColor(49, 119, 88);
    inline const sf::Color DangerButtonColor(145, 61, 65);
}

namespace StartScreenConfig {
    inline constexpr float SettingsPadding = 24.f;
    inline constexpr float SettingsButtonSize = 78.f;
    inline constexpr float SettingsIconSize = 48.f;
    inline constexpr float QuitButtonSize = 78.f;
    inline constexpr float QuitIconSize = 48.f;
    inline constexpr float CoinCounterWidth = 210.f;
    inline constexpr float ButtonWidth = 360.f;
    inline constexpr float ButtonHeight = 72.f;
    inline constexpr float ButtonGap = 28.f;
    inline constexpr float ButtonOutlineThickness = 2.f;
    inline constexpr unsigned int ButtonTextSize = 36;
    inline const sf::Color ButtonColor(16, 20, 28, 175);
    inline const sf::Color ButtonHoverColor(49, 119, 88, 220);
    inline const sf::Color ButtonOutlineColor(244, 232, 178, 215);
    inline const sf::Color ButtonTextColor(255, 255, 255);
    inline const sf::Color SettingsButtonColor(16, 20, 28, 185);
    inline const sf::Color SettingsButtonHoverColor(49, 119, 88, 225);
    inline const sf::Color QuitButtonColor(145, 61, 65, 225);
    inline const sf::Color QuitButtonHoverColor(210, 62, 70, 245);
    inline const sf::Color QuitIconColor(255, 255, 255);
}

namespace ShopScreenConfig {
    inline constexpr float TopPadding = 28.f;
    inline constexpr float SidePadding = 42.f;
    inline constexpr float BackButtonWidth = 170.f;
    inline constexpr float BackButtonHeight = 58.f;
    inline constexpr float CategoryTop = 110.f;
    inline constexpr float CategoryWidth = 300.f;
    inline constexpr float CategoryHeight = 56.f;
    inline constexpr float CategoryGap = 18.f;
    inline constexpr float GridTop = 205.f;
    inline constexpr float TileWidth = 300.f;
    inline constexpr float TileHeight = 365.f;
    inline constexpr float TileGap = 34.f;
    inline constexpr float ThumbnailSize = 220.f;
    inline constexpr float LockSize = 74.f;
    inline constexpr float WarningDotRadius = 11.f;
    inline constexpr unsigned int TitleTextSize = 52;
    inline constexpr unsigned int ButtonTextSize = 28;
    inline constexpr unsigned int TileNameSize = 22;
    inline constexpr unsigned int TilePriceSize = 18;
    inline const sf::Color BackgroundColor(11, 15, 22, 235);
    inline const sf::Color PanelColor(16, 20, 28, 190);
    inline const sf::Color PanelHoverColor(34, 45, 60, 220);
    inline const sf::Color SelectedColor(49, 119, 88, 230);
    inline const sf::Color OutlineColor(244, 232, 178, 205);
    inline const sf::Color TextColor(245, 247, 252);
    inline const sf::Color MutedTextColor(190, 198, 210);
    inline const sf::Color LockedColor(255, 255, 255, 110);
    inline const sf::Color WarningColor(235, 139, 39);
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
