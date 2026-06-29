#include "Resources/DataManager.h"

#include "Shop/ShopData.h"
#include "external/json.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>

using Json = nlohmann::json;

void from_json(const Json& json, LevelConfig& level);
void from_json(const Json& json, ShopAsset& asset);
void from_json(const Json& json, UserData& userData);
void to_json(Json& json, const UserData& userData);

namespace {
const std::filesystem::path GameDataPath = "data/gameData.json";
const std::filesystem::path UserDataPath = "userData.json";

Json fetchJson(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("DataManager: cannot open JSON file: " + path.generic_string());
    }

    try {
        return Json::parse(file);
    } catch (const Json::parse_error& error) {
        throw std::runtime_error("DataManager: JSON parse error in "
            + path.generic_string() + ": " + error.what());
    }
}

std::vector<LevelConfig> parseLevels(const Json& root) {
    return root.at("stages").get<std::vector<LevelConfig>>();
}

std::vector<ShopAsset> parseShopAssets(const Json& root) {
    std::vector<ShopAsset> assets;
    const Json&            assetsValue = root.at("shopAssets");

    for (ShopAssetType type : allShopAssetTypes()) {
        const Json& group = assetsValue.at(shopAssetTypeToKey(type));
        for (const Json& value : group) {
            ShopAsset asset = value.get<ShopAsset>();
            asset.type      = type;
            assets.push_back(asset);
        }
    }

    return assets;
}

std::set<int> parseIdArray(const Json& object, const std::string& key) {
    std::set<int> ids;
    const auto    found = object.find(key);
    if (found == object.end() || !found->is_array()) {
        return ids;
    }

    for (const Json& value : *found) {
        if (!value.is_number_integer()) {
            continue;
        }

        ids.insert(value.get<int>());
    }

    return ids;
}

const ShopAsset* findDefaultAsset(const std::vector<ShopAsset>& catalog, ShopAssetType type) {
    for (const ShopAsset& asset : catalog) {
        if (asset.type == type && asset.isDefault) {
            return &asset;
        }
    }

    return nullptr;
}
}

void from_json(const Json& json, LevelConfig& level) {
    level.enemyCount    = json.value("enemyCount", 0);
    level.tankChance    = json.value("tankChance", 0.f);
    level.spawnInterval = json.value("spawnInterval", 0.f);
    level.batteryDrain  = json.value("batteryDrain", 0.f);
    level.startAmmo     = json.value("startAmmo", 0);
}

void from_json(const Json& json, ShopAsset& asset) {
    const std::string name = json.value("name", "");
    asset.id              = json.value("id", 0);
    asset.title           = json.value("title", name);
    asset.price           = json.value("price", 0);
    asset.file            = json.value("file", "");
    asset.extra           = json.value("extra", 0);
    asset.isDefault       = json.value("default", false);
    asset.description     = json.value("description", "");
}

void from_json(const Json& json, UserData& userData) {
    if (!json.is_object()) {
        return;
    }

    userData.coins = json.value("coins", 0);

    const auto owned = json.find("ownedAssets");
    if (owned != json.end() && owned->is_object()) {
        for (ShopAssetType type : allShopAssetTypes()) {
            userData.ownedAssets[type] = parseIdArray(*owned, shopAssetTypeToKey(type));
        }
    }

    const auto selected = json.find("selectedAssets");
    if (selected != json.end() && selected->is_object()) {
        for (ShopAssetType type : allShopAssetTypes()) {
            const auto found = selected->find(shopAssetTypeToKey(type));
            if (found != selected->end() && found->is_number_integer()) {
                userData.selectedAssets[type] = found->get<int>();
            }
        }
    }
}

void to_json(Json& json, const UserData& userData) {
    Json ownedAssets    = Json::object();
    Json selectedAssets = Json::object();

    for (ShopAssetType type : allShopAssetTypes()) {
        const std::string key = shopAssetTypeToKey(type);

        const auto owned = userData.ownedAssets.find(type);
        ownedAssets[key] = owned == userData.ownedAssets.end()
            ? Json::array()
            : Json(owned->second);

        const auto selected = userData.selectedAssets.find(type);
        selectedAssets[key] = selected == userData.selectedAssets.end() ? 0 : selected->second;
    }

    json = {
        { "coins", std::max(0, userData.coins) },
        { "ownedAssets", ownedAssets },
        { "selectedAssets", selectedAssets }
    };
}

#pragma region Public
DataManager& DataManager::instance() {
    static DataManager manager;
    return manager;
}

void DataManager::load() {
    m_gameData = fetchGameData();
    m_userData = fetchUserData();
}

void DataManager::updateUserData(const UserData& userData) {
    m_userData = normalizeUserData(userData);
}

void DataManager::saveUserData() {
    writeUserData(m_userData);
}
#pragma endregion

#pragma region Fetching
GameData DataManager::fetchGameData() const {
    const Json root = fetchJson(GameDataPath);
    if (!root.is_object()) {
        throw std::runtime_error("DataManager: game data root must be an object");
    }

    GameData gameData;
    gameData.stages     = parseLevels(root);
    gameData.shopAssets = parseShopAssets(root);

    return gameData;
}

UserData DataManager::fetchUserData() const {
    UserData userData;

    try {
        userData = fetchJson(UserDataPath).get<UserData>();
    } catch (const std::exception&) {
        userData = UserData{};
    }

    return normalizeUserData(userData);
}

UserData DataManager::normalizeUserData(const UserData& userData) const {
    UserData normalized = userData;
    normalized.coins    = std::max(0, normalized.coins);

    for (ShopAssetType type : allShopAssetTypes()) {
        const ShopAsset* defaultAsset = findDefaultAsset(m_gameData.shopAssets, type);
        if (!defaultAsset) {
            continue;
        }

        std::set<int> validIds;
        for (const ShopAsset& asset : m_gameData.shopAssets) {
            if (asset.type == type) {
                validIds.insert(asset.id);
            }
        }

        std::erase_if(normalized.ownedAssets[type], [&validIds](int id) {
            return !validIds.contains(id);
        });

        normalized.ownedAssets[type].insert(defaultAsset->id);

        const int selectedId = normalized.selectedAssets.contains(type)
            ? normalized.selectedAssets[type]
            : defaultAsset->id;

        if (validIds.contains(selectedId) && normalized.ownedAssets[type].contains(selectedId)) {
            normalized.selectedAssets[type] = selectedId;
        } else {
            normalized.selectedAssets[type] = defaultAsset->id;
        }
    }

    return normalized;
}

void DataManager::writeUserData(const UserData& userData) const {
    std::ofstream file(UserDataPath);
    if (!file) {
        return;
    }

    file << Json(userData).dump(2) << '\n';
}
#pragma endregion
