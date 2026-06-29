#pragma once

#include "Enums/ShopAssetType.h"

#include <map>
#include <set>

/// @brief Player-owned persistent state.
struct UserData {
    int coins = 0;
    bool muted = false;
    float maxVolume = 100.f;
    std::map<ShopAssetType, std::set<int>> ownedAssets;
    std::map<ShopAssetType, int> selectedAssets;
};
