#pragma once

#include "Enums/ShopAssetType.h"
#include "Gameplay/LevelConfig.h"

#include <map>
#include <set>
#include <string>
#include <vector>

struct ShopAsset {
    int id = 0;
    ShopAssetType type = ShopAssetType::CommandVehicle;
    std::string name;
    int price = 0;
    std::string file;
    bool isDefault = false;
    std::string description;
};

struct UserData {
    int coins = 0;
    std::map<ShopAssetType, std::set<int>> ownedAssets;
    std::map<ShopAssetType, int> selectedAssets;
};

struct GameData {
    std::vector<LevelConfig> stages;
    std::vector<ShopAsset> shopAssets;
    int uncatalogedShopFiles = 0;
};

std::string shopAssetTypeToKey(ShopAssetType type);
std::string shopAssetTypeToTitle(ShopAssetType type);
bool shopAssetTypeFromKey(const std::string& key, ShopAssetType& type);
std::vector<ShopAssetType> allShopAssetTypes();
