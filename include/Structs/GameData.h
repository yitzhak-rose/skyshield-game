#pragma once

#include "Structs/LevelConfig.h"
#include "Structs/ShopAsset.h"

#include <vector>

/// @brief Immutable app-owned data loaded from gameData.json.
struct GameData {
    std::vector<LevelConfig> stages;
    std::vector<ShopAsset> shopAssets;
};
