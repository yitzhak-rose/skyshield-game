#pragma once

#include "Enums/ShopAssetType.h"

#include <string>

/// @brief One purchasable/selectable asset from the shop catalog.
struct ShopAsset {
    int id = 0;
    ShopAssetType type = ShopAssetType::CommandVehicle;
    std::string title;
    int price = 0;
    std::string file;
    int extra = 0;
    bool isDefault = false;
    std::string description;
};
