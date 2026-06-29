#pragma once

#include "Enums/Fonts.h"
#include "Enums/GameObjects.h"
#include "Enums/Sounds.h"
#include "Structs/ResourceInfo.h"

#include <array>

namespace Resources {
    /// @brief Statically defined configuration of game object textures and their paths.
    inline const std::array<ResourceInfo<GameObjects>, 22> imageResources = { {
    { GameObjects::ArrowLeft, "images/game/arrow-left.png" },
    { GameObjects::ArrowRight, "images/game/arrow-right.png" },
    { GameObjects::Background, "images/default/background.png" },
    { GameObjects::Battery, "images/game/battery.png" },
    { GameObjects::Bomb, "images/game/bomb.png" },
    { GameObjects::Bullet, "images/game/bullet.png" },
    { GameObjects::ChevronBack, "images/game/chevron-back.png" },
    { GameObjects::Coin, "images/game/coin.png" },
    { GameObjects::CommandCar, "images/default/commandCar.png" },
    { GameObjects::Drone, "images/default/drone.png" },
    { GameObjects::Enemy, "images/game/enemy.png" },
    { GameObjects::Gear, "images/game/gear.png" },
    { GameObjects::HelpBackground, "images/game/help-background.png" },
    { GameObjects::Lock, "images/game/lock.png" },
    { GameObjects::Mute, "images/game/mute.png" },
    { GameObjects::SmallEnemy, "images/game/smallEnemy.png" },
    { GameObjects::Spacebar, "images/game/spacebar.png" },
    { GameObjects::StartBackground, "images/default/start-background.png" },
    { GameObjects::VolumeHigh, "images/game/volume-high.png" },
    { GameObjects::VolumeLow, "images/game/volume-low.png" },
    { GameObjects::XSquare, "images/game/x.square.png" },
    { GameObjects::Explosion, "images/game/explosion.png" },

} };

    /// @brief Statically defined configuration of fonts and their paths.
    inline const std::array<ResourceInfo<Fonts>, 3> fontResources = { {
    { Fonts::BattleGrid, "fonts/BattleGridFree.ttf" },
    { Fonts::StencilForce, "fonts/StencilForce.otf" },
    { Fonts::GameBattles, "fonts/game-battles-2.ttf" },

} };

    /// @brief Statically defined configuration of sound assets and their paths.
    inline const std::array<ResourceInfo<Sounds>, 4> soundResources = { {
    { Sounds::Background, "sounds/default/background-sound.mp3" },
    { Sounds::LevelFinished, "sounds/game/level-finished.mp3" },
    { Sounds::MissionFailed, "sounds/game/mission-failed.mp3" },
    { Sounds::StartBackground, "sounds/game/start-background-sound.mp3" },

} };
}
