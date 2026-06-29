#include "Resources/DataManager.h"
#include "Utilities/JsonUtils.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>

// Private DataManager helpers for the game-specific JSON schema.

std::string DataManager::requireTitle(const JsonUtils::Json& object, const std::string& context) {
    const std::string title = JsonUtils::optionalString(object, "title");
    if (!title.empty())
        return title;

    return JsonUtils::requireString(object, "name", context);
}

LevelConfig DataManager::parseLevel(const JsonUtils::Json& value, std::size_t stageIndex) {
    if (!value.is_object()) {
        throw std::runtime_error("DataManager: stage "
            + std::to_string(stageIndex + 1) + " must be an object");
    }

    const std::string context = "stage " + std::to_string(stageIndex + 1);
    return {
        JsonUtils::requireInt(value, "enemyCount", context),
        JsonUtils::requireFloat(value, "tankChance", context),
        JsonUtils::requireFloat(value, "spawnInterval", context),
        JsonUtils::requireFloat(value, "batteryDrain", context),
        JsonUtils::requireInt(value, "startAmmo", context)
    };
}

ShopAsset DataManager::parseShopAsset(const JsonUtils::Json& value) {
    if (!value.is_object())
        throw std::runtime_error("DataManager: shop asset must be an object");

    const int id = JsonUtils::requireInt(value, "id", "shop asset");
    if (id <= 0)
        throw std::runtime_error("DataManager: shop asset id must be positive");

    ShopAssetType type = ShopAssetType::CommandVehicle;
    const std::string typeKey = JsonUtils::requireString(value, "type", "shop asset " + std::to_string(id));
    if (!shopAssetTypeFromKey(typeKey, type))
        throw std::runtime_error("DataManager: unknown shop asset type: " + typeKey);

    ShopAsset asset;
    asset.id = id;
    asset.type = type;
    asset.title = requireTitle(value, "shop asset " + std::to_string(id));
    asset.price = JsonUtils::requireInt(value, "price", "shop asset " + std::to_string(id));
    asset.file = JsonUtils::requireString(value, "file", "shop asset " + std::to_string(id));
    asset.extra = JsonUtils::requireInt(value, "extra", "shop asset " + std::to_string(id));
    asset.isDefault = JsonUtils::optionalBool(value, "default", false);
    asset.description = JsonUtils::optionalString(value, "description");

    if (asset.price < 0)
        throw std::runtime_error("DataManager: shop asset price cannot be negative");
    if (asset.extra < 0)
        throw std::runtime_error("DataManager: shop asset extra cannot be negative");
    if (!std::filesystem::exists(asset.file))
        throw std::runtime_error("DataManager: missing shop asset file: " + asset.file);

    return asset;
}

std::vector<LevelConfig> DataManager::parseLevels(const JsonUtils::Json& root, const std::string& context) {
    const JsonUtils::Json& stagesValue = JsonUtils::requireField(root, "stages", context);
    if (!stagesValue.is_array())
        throw std::runtime_error("DataManager: \"stages\" must be an array");

    std::vector<LevelConfig> stages;
    stages.reserve(stagesValue.size());
    for (std::size_t index = 0; index < stagesValue.size(); ++index) {
        stages.push_back(parseLevel(stagesValue[index], index));
    }

    return stages;
}

std::vector<ShopAsset> DataManager::parseShopAssets(const JsonUtils::Json& root) {
    const JsonUtils::Json& assetsValue = JsonUtils::requireField(root, "shopAssets", "game data root");
    if (!assetsValue.is_array())
        throw std::runtime_error("DataManager: \"shopAssets\" must be an array");

    std::vector<ShopAsset> assets;
    std::set<int> ids;
    std::set<ShopAssetType> defaults;

    for (const JsonUtils::Json& value : assetsValue) {
        ShopAsset asset = parseShopAsset(value);
        if (!ids.insert(asset.id).second)
            throw std::runtime_error("DataManager: duplicate shop asset id: " + std::to_string(asset.id));

        if (asset.isDefault) {
            if (asset.price != 0)
                throw std::runtime_error("DataManager: default shop assets must be free");
            defaults.insert(asset.type);
        }

        assets.push_back(std::move(asset));
    }

    for (ShopAssetType type : allShopAssetTypes()) {
        if (!defaults.contains(type))
            throw std::runtime_error("DataManager: missing default shop asset for " + shopAssetTypeToKey(type));
    }

    return assets;
}

bool DataManager::isHiddenShopCompanionFile(const std::filesystem::path& path) {
    return path.filename().generic_string().starts_with("start-background");
}

int DataManager::countUncatalogedShopFiles(const std::vector<ShopAsset>& catalog) {
    std::set<std::string> catalogFiles;
    for (const ShopAsset& asset : catalog)
        catalogFiles.insert(asset.file);

    int count = 0;
    const std::array<std::string, 2> folders = { "images/shop", "sounds/shop" };
    for (const std::string& folder : folders) {
        if (!std::filesystem::exists(folder))
            continue;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
            if (!entry.is_regular_file())
                continue;
            if (isHiddenShopCompanionFile(entry.path()))
                continue;

            const std::string path = entry.path().generic_string();
            if (!catalogFiles.contains(path))
                ++count;
        }
    }

    return count;
}

const ShopAsset* DataManager::findDefaultAsset(const std::vector<ShopAsset>& catalog, ShopAssetType type) {
    for (const ShopAsset& asset : catalog) {
        if (asset.type == type && asset.isDefault)
            return &asset;
    }

    return nullptr;
}

void DataManager::normalizeUserData(UserData& userData, const std::vector<ShopAsset>& catalog) {
    userData.coins = std::max(0, userData.coins);

    for (ShopAssetType type : allShopAssetTypes()) {
        const ShopAsset* defaultAsset = findDefaultAsset(catalog, type);
        if (!defaultAsset)
            continue;

        std::set<int> validIds;
        for (const ShopAsset& asset : catalog) {
            if (asset.type == type)
                validIds.insert(asset.id);
        }

        std::erase_if(userData.ownedAssets[type], [&validIds](int id) {
            return !validIds.contains(id);
        });

        userData.ownedAssets[type].insert(defaultAsset->id);

        const int selectedId = userData.selectedAssets.contains(type)
            ? userData.selectedAssets[type]
            : defaultAsset->id;

        if (validIds.contains(selectedId) && userData.ownedAssets[type].contains(selectedId))
            userData.selectedAssets[type] = selectedId;
        else
            userData.selectedAssets[type] = defaultAsset->id;
    }
}

std::set<int> DataManager::parseIdArray(const JsonUtils::Json& object, const std::string& key) {
    std::set<int> ids;
    const auto found = object.find(key);
    if (found == object.end() || !found->is_array())
        return ids;

    for (const JsonUtils::Json& value : *found) {
        if (!value.is_number_integer())
            continue;

        const long long id = value.get<long long>();
        if (id > 0 && id <= static_cast<long long>(std::numeric_limits<int>::max()))
            ids.insert(static_cast<int>(id));
    }

    return ids;
}

JsonUtils::Json DataManager::userDataToJson(const UserData& userData) {
    JsonUtils::Json ownedAssets = JsonUtils::Json::object();
    JsonUtils::Json selectedAssets = JsonUtils::Json::object();

    for (ShopAssetType type : allShopAssetTypes()) {
        const std::string key = shopAssetTypeToKey(type);

        const auto owned = userData.ownedAssets.find(type);
        ownedAssets[key] = owned == userData.ownedAssets.end()
            ? JsonUtils::Json::array()
            : JsonUtils::Json(owned->second);

        const auto selected = userData.selectedAssets.find(type);
        selectedAssets[key] = selected == userData.selectedAssets.end() ? 0 : selected->second;
    }

    return {
        { "coins", std::max(0, userData.coins) },
        { "ownedAssets", ownedAssets },
        { "selectedAssets", selectedAssets }
    };
}

UserData DataManager::parseUserData(const JsonUtils::Json& root) {
    UserData userData;
    if (!root.is_object())
        return userData;

    const auto coins = root.find("coins");
    if (coins != root.end() && coins->is_number_integer()) {
        const long long value = coins->get<long long>();
        if (value >= 0 && value <= static_cast<long long>(std::numeric_limits<int>::max()))
            userData.coins = static_cast<int>(value);
    }

    const auto owned = root.find("ownedAssets");
    if (owned != root.end() && owned->is_object()) {
        for (ShopAssetType type : allShopAssetTypes())
            userData.ownedAssets[type] = parseIdArray(*owned, shopAssetTypeToKey(type));
    }

    const auto selected = root.find("selectedAssets");
    if (selected != root.end() && selected->is_object()) {
        for (ShopAssetType type : allShopAssetTypes()) {
            const auto found = selected->find(shopAssetTypeToKey(type));
            if (found == selected->end() || !found->is_number_integer())
                continue;

            const long long id = found->get<long long>();
            if (id > 0 && id <= static_cast<long long>(std::numeric_limits<int>::max()))
                userData.selectedAssets[type] = static_cast<int>(id);
        }
    }

    return userData;
}

// Public DataManager API.

DataManager& DataManager::instance() {
    static DataManager manager;
    return manager;
}

void DataManager::configure(std::string gameDataPath, std::string userDataPath) {
    m_gameDataPath = std::move(gameDataPath);
    m_userDataPath = std::move(userDataPath);
    m_hasGameDataWriteTime = false;
    m_hasUserDataWriteTime = false;
}

void DataManager::load() {
    m_gameData = loadGameData(m_gameDataPath);
    m_userData = loadUserData(m_userDataPath, m_gameData.shopAssets);
    rememberFileTime(m_gameDataPath, m_gameDataWriteTime, m_hasGameDataWriteTime);
    rememberFileTime(m_userDataPath, m_userDataWriteTime, m_hasUserDataWriteTime);
}

bool DataManager::refreshIfChanged() {
    bool changed = false;

    if (hasFileChanged(m_gameDataPath, m_gameDataWriteTime, m_hasGameDataWriteTime)) {
        m_gameData = loadGameData(m_gameDataPath);
        m_userData = loadUserData(m_userDataPath, m_gameData.shopAssets);
        rememberFileTime(m_gameDataPath, m_gameDataWriteTime, m_hasGameDataWriteTime);
        rememberFileTime(m_userDataPath, m_userDataWriteTime, m_hasUserDataWriteTime);
        changed = true;
    } else if (hasFileChanged(m_userDataPath, m_userDataWriteTime, m_hasUserDataWriteTime)) {
        m_userData = loadUserData(m_userDataPath, m_gameData.shopAssets);
        rememberFileTime(m_userDataPath, m_userDataWriteTime, m_hasUserDataWriteTime);
        changed = true;
    }

    return changed;
}

void DataManager::setUserData(const UserData& userData) {
    m_userData = userData;
    normalizeUserData(m_userData, m_gameData.shopAssets);
}

void DataManager::saveUserData() {
    saveUserData(m_userDataPath, m_userData);
    rememberFileTime(m_userDataPath, m_userDataWriteTime, m_hasUserDataWriteTime);
}

std::vector<LevelConfig> DataManager::loadLevels(const std::string& path) const {
    const JsonUtils::Json root = JsonUtils::readJsonFile(path);
    std::vector<LevelConfig> stages = parseLevels(root, "level JSON root");
    for (std::size_t index = 0; index < stages.size(); ++index)
        validateLevel(stages[index], index);

    if (stages.empty())
        throw std::runtime_error("DataManager: no valid stages found in: " + path);

    return stages;
}

GameData DataManager::loadGameData(const std::string& path) const {
    const JsonUtils::Json root = JsonUtils::readJsonFile(path);
    if (!root.is_object())
        throw std::runtime_error("DataManager: game data JSON root must be an object");

    GameData gameData;
    gameData.stages = parseLevels(root, "game data root");
    for (std::size_t index = 0; index < gameData.stages.size(); ++index)
        validateLevel(gameData.stages[index], index);

    if (gameData.stages.empty())
        throw std::runtime_error("DataManager: no valid stages found in: " + path);

    gameData.shopAssets = parseShopAssets(root);
    gameData.uncatalogedShopFiles = countUncatalogedShopFiles(gameData.shopAssets);

    return gameData;
}

UserData DataManager::loadUserData(const std::string& path, const std::vector<ShopAsset>& catalog) const {
    UserData userData;

    try {
        userData = parseUserData(JsonUtils::readJsonFile(path));
    } catch (const std::exception&) {
        userData = UserData{};
    }

    normalizeUserData(userData, catalog);
    return userData;
}

void DataManager::saveUserData(const std::string& path, const UserData& userData) {
    std::ofstream file(path);
    if (!file)
        return;

    file << userDataToJson(userData).dump(2) << '\n';
}

int DataManager::loadCoins(const std::string& path) const {
    try {
        const JsonUtils::Json root = JsonUtils::readJsonFile(path);
        const auto coins = root.find("coins");
        if (coins == root.end() || !coins->is_number_integer())
            return 0;

        const long long value = coins->get<long long>();
        if (value < 0)
            return 0;
        if (value > static_cast<long long>(std::numeric_limits<int>::max()))
            return std::numeric_limits<int>::max();

        return static_cast<int>(value);
    } catch (const std::exception&) {
        return 0;
    }
}

void DataManager::saveCoins(const std::string& path, int coins) {
    std::ofstream file(path);
    if (!file)
        return;

    file << JsonUtils::Json{ { "coins", std::max(0, coins) } }.dump(2) << '\n';
}

// Private DataManager helpers.

void DataManager::validateLevel(const LevelConfig& config, std::size_t stageIndex) {
    const auto fail = [stageIndex](const std::string& why) {
        throw std::runtime_error("DataManager: stage "
            + std::to_string(stageIndex + 1) + " - " + why);
    };

    if (config.enemyCount < 1)
        fail("enemyCount must be >= 1");
    if (config.tankChance < 0.f || config.tankChance > 1.f)
        fail("tankChance must be in [0, 1]");
    if (config.spawnInterval <= 0.f)
        fail("spawnInterval must be > 0");
    if (config.batteryDrain < 0.f)
        fail("batteryDrain must be >= 0");
    if (config.startAmmo < 1)
        fail("startAmmo must be >= 1");
}

std::filesystem::file_time_type DataManager::fileTime(const std::string& path) const {
    std::error_code error;
    const auto time = std::filesystem::last_write_time(path, error);
    return error ? std::filesystem::file_time_type{} : time;
}

bool DataManager::hasFileChanged(const std::string& path,
                                 std::filesystem::file_time_type& knownTime,
                                 bool& hasKnownTime) const {
    const std::filesystem::file_time_type currentTime = fileTime(path);
    if (!hasKnownTime) {
        knownTime = currentTime;
        hasKnownTime = true;
        return false;
    }

    return currentTime != knownTime;
}

void DataManager::rememberFileTime(const std::string& path,
                                   std::filesystem::file_time_type& knownTime,
                                   bool& hasKnownTime) const {
    knownTime = fileTime(path);
    hasKnownTime = true;
}
