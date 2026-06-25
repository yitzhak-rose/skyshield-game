#include "Gameplay/ShopData.h"

std::string shopAssetTypeToKey(ShopAssetType type) {
    switch (type) {
        case ShopAssetType::CommandVehicle:
            return "command_vehicle";
        case ShopAssetType::Drone:
            return "drone";
        case ShopAssetType::Background:
            return "background";
        case ShopAssetType::BackgroundMusic:
            return "background_music";
        case ShopAssetType::Font:
            return "font";
    }

    return "";
}

std::string shopAssetTypeToTitle(ShopAssetType type) {
    switch (type) {
        case ShopAssetType::CommandVehicle:
            return "Command Vehicle";
        case ShopAssetType::Drone:
            return "Drone";
        case ShopAssetType::Background:
            return "Background";
        case ShopAssetType::BackgroundMusic:
            return "Music";
        case ShopAssetType::Font:
            return "Font";
    }

    return "";
}

bool shopAssetTypeFromKey(const std::string& key, ShopAssetType& type) {
    for (ShopAssetType candidate : allShopAssetTypes()) {
        if (shopAssetTypeToKey(candidate) == key) {
            type = candidate;
            return true;
        }
    }

    return false;
}

std::vector<ShopAssetType> allShopAssetTypes() {
    return {
        ShopAssetType::CommandVehicle,
        ShopAssetType::Drone,
        ShopAssetType::Background,
        ShopAssetType::BackgroundMusic,
        ShopAssetType::Font
    };
}
