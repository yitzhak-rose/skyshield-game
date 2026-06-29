#pragma once

#include "Enums/ShopAssetType.h"
#include "Structs/GameData.h"
#include "Structs/ShopAsset.h"
#include "Structs/UserData.h"

#include <string>
#include <vector>

std::string shopAssetTypeToKey(ShopAssetType type);
std::string shopAssetTypeToTitle(ShopAssetType type);
bool shopAssetTypeFromKey(const std::string& key, ShopAssetType& type);
std::vector<ShopAssetType> allShopAssetTypes();
