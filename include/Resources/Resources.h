#pragma once

#include <array>
#include <string>
#include "Enums/Fonts.h"
#include "Enums/GameObjects.h"
#include "Enums/Sounds.h"

/// @brief Generic metadata struct to associate a resource key (enum) with its default file path.
template <typename Key>
struct ResourceInfo {
    Key type;           ///< The identifier/key for the resource.
    std::string path;   ///< The file path to load the resource from.
};

namespace Resources {
    /// @brief Statically defined configuration of game object textures and their paths.
    inline const std::array<ResourceInfo<GameObjects>, 12> imageResources = { {
    { GameObjects::Background, "background.png" },
    { GameObjects::Battery, "battery.png" },
    { GameObjects::Bomb, "bomb.png" },
    { GameObjects::Bullet, "bullet.png" },
    { GameObjects::Coin, "coin.png" },
    { GameObjects::CommandCar, "commandCar.png" },
    { GameObjects::Drone, "drone.png" },
    { GameObjects::Enemy, "enemy.png" },
    { GameObjects::Gear, "gear.png" },
    { GameObjects::SmallEnemy, "smallEnemy.png" },
    { GameObjects::StartBackground, "start-background.png" },
    { GameObjects::XSquare, "x.square.png" },

} };

    /// @brief Statically defined configuration of fonts and their paths.
    inline const std::array<ResourceInfo<Fonts>, 3> fontResources = { {
    { Fonts::BattleGrid, "BattleGridFree.ttf" },
    { Fonts::StencilForce, "StencilForce.otf" },
    { Fonts::GameBattles, "game-battles-2.ttf" },

} };

    /// @brief Statically defined configuration of sound assets and their paths.
    inline const std::array<ResourceInfo<Sounds>, 4> soundResources = { {
    { Sounds::Background, "background-sound.mp3" },
    { Sounds::LevelFinished, "level-finished.mp3" },
    { Sounds::MissionFailed, "mission-failed.mp3" },
    { Sounds::StartBackground, "start-background-sound.mp3" },

} };
}
