#include "Gameplay/DataManager.h"

#include <algorithm>
#include <array>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

namespace {
class JsonValue {
public:
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;

    JsonValue() = default;
    explicit JsonValue(std::nullptr_t) {}
    explicit JsonValue(bool value) : m_value(value) {}
    explicit JsonValue(double value) : m_value(value) {}
    explicit JsonValue(std::string value) : m_value(std::move(value)) {}
    explicit JsonValue(Array value) : m_value(std::move(value)) {}
    explicit JsonValue(Object value) : m_value(std::move(value)) {}

    bool isNumber() const { return std::holds_alternative<double>(m_value); }
    bool isBool() const { return std::holds_alternative<bool>(m_value); }
    bool isString() const { return std::holds_alternative<std::string>(m_value); }
    bool isArray() const { return std::holds_alternative<Array>(m_value); }
    bool isObject() const { return std::holds_alternative<Object>(m_value); }

    double number() const { return std::get<double>(m_value); }
    bool boolean() const { return std::get<bool>(m_value); }
    const std::string& string() const { return std::get<std::string>(m_value); }
    const Array& array() const { return std::get<Array>(m_value); }
    const Object& object() const { return std::get<Object>(m_value); }

private:
    std::variant<std::nullptr_t, bool, double, std::string, Array, Object> m_value;
};

class JsonParser {
public:
    explicit JsonParser(std::string text) : m_text(std::move(text)) {}

    JsonValue parse() {
        JsonValue value = parseValue();
        skipWhitespace();

        if (!isAtEnd())
            fail("unexpected trailing data");

        return value;
    }

private:
    JsonValue parseValue() {
        skipWhitespace();

        if (isAtEnd())
            fail("expected a JSON value");

        const char current = peek();
        if (current == '{')
            return JsonValue(parseObject());
        if (current == '[')
            return JsonValue(parseArray());
        if (current == '"')
            return JsonValue(parseString());
        if (current == '-' || isDigit(current))
            return JsonValue(parseNumber());
        if (matchLiteral("true"))
            return JsonValue(true);
        if (matchLiteral("false"))
            return JsonValue(false);
        if (matchLiteral("null"))
            return JsonValue(nullptr);

        fail("unexpected character");
    }

    JsonValue::Object parseObject() {
        consume('{');
        JsonValue::Object object;
        skipWhitespace();

        if (consumeIf('}'))
            return object;

        while (true) {
            skipWhitespace();
            if (peek() != '"')
                fail("expected an object key");

            std::string key = parseString();
            skipWhitespace();
            consume(':');
            object.emplace(std::move(key), parseValue());
            skipWhitespace();

            if (consumeIf('}'))
                return object;

            consume(',');
        }
    }

    JsonValue::Array parseArray() {
        consume('[');
        JsonValue::Array array;
        skipWhitespace();

        if (consumeIf(']'))
            return array;

        while (true) {
            array.push_back(parseValue());
            skipWhitespace();

            if (consumeIf(']'))
                return array;

            consume(',');
        }
    }

    std::string parseString() {
        consume('"');
        std::string result;

        while (!isAtEnd()) {
            const char current = advance();

            if (current == '"')
                return result;

            if (current == '\\') {
                if (isAtEnd())
                    fail("unterminated string escape");

                const char escaped = advance();
                switch (escaped) {
                    case '"':
                    case '\\':
                    case '/':
                        result.push_back(escaped);
                        break;
                    case 'b':
                        result.push_back('\b');
                        break;
                    case 'f':
                        result.push_back('\f');
                        break;
                    case 'n':
                        result.push_back('\n');
                        break;
                    case 'r':
                        result.push_back('\r');
                        break;
                    case 't':
                        result.push_back('\t');
                        break;
                    default:
                        fail("unsupported string escape");
                }
                continue;
            }

            result.push_back(current);
        }

        fail("unterminated string");
    }

    double parseNumber() {
        const std::size_t start = m_position;

        consumeIf('-');

        if (consumeIf('0')) {
            // A leading zero must stand alone before a fraction or exponent.
        } else {
            consumeDigits();
        }

        if (consumeIf('.'))
            consumeDigits();

        if (consumeIf('e') || consumeIf('E')) {
            if (!consumeIf('+'))
                consumeIf('-');
            consumeDigits();
        }

        const std::string numberText = m_text.substr(start, m_position - start);
        char* end = nullptr;
        errno = 0;
        const double number = std::strtod(numberText.c_str(), &end);

        if (errno == ERANGE || end != numberText.c_str() + numberText.size() || !std::isfinite(number))
            fail("invalid number");

        return number;
    }

    void consumeDigits() {
        if (isAtEnd() || !isDigit(peek()))
            fail("expected a digit");

        while (!isAtEnd() && isDigit(peek()))
            ++m_position;
    }

    bool matchLiteral(const std::string& literal) {
        if (m_text.compare(m_position, literal.size(), literal) != 0)
            return false;

        m_position += literal.size();
        return true;
    }

    void skipWhitespace() {
        while (!isAtEnd()) {
            const char current = peek();
            if (current != ' ' && current != '\n' && current != '\r' && current != '\t')
                return;

            ++m_position;
        }
    }

    void consume(char expected) {
        if (!consumeIf(expected))
            fail(std::string("expected '") + expected + "'");
    }

    bool consumeIf(char expected) {
        if (isAtEnd() || peek() != expected)
            return false;

        ++m_position;
        return true;
    }

    char advance() {
        const char current = peek();
        ++m_position;
        return current;
    }

    char peek() const {
        if (isAtEnd())
            fail("unexpected end of file");

        return m_text[m_position];
    }

    bool isAtEnd() const {
        return m_position >= m_text.size();
    }

    static bool isDigit(char value) {
        return value >= '0' && value <= '9';
    }

    [[noreturn]] void fail(const std::string& message) const {
        throw std::runtime_error("DataManager JSON parse error near character "
            + std::to_string(m_position) + ": " + message);
    }

    std::string m_text;
    std::size_t m_position = 0;
};

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("DataManager: cannot open JSON file: " + path);

    return {
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };
}

const JsonValue& requireField(const JsonValue::Object& object,
                              const std::string& key,
                              const std::string& context) {
    const auto found = object.find(key);
    if (found == object.end())
        throw std::runtime_error("DataManager: missing \"" + key + "\" in " + context);

    return found->second;
}

int requireInt(const JsonValue::Object& object,
               const std::string& key,
               const std::string& context) {
    const JsonValue& value = requireField(object, key, context);
    if (!value.isNumber())
        throw std::runtime_error("DataManager: \"" + key + "\" must be a number in " + context);

    const double number = value.number();
    if (std::trunc(number) != number ||
        number < static_cast<double>(std::numeric_limits<int>::min()) ||
        number > static_cast<double>(std::numeric_limits<int>::max())) {
        throw std::runtime_error("DataManager: \"" + key + "\" must be an integer in " + context);
    }

    return static_cast<int>(number);
}

float requireFloat(const JsonValue::Object& object,
                   const std::string& key,
                   const std::string& context) {
    const JsonValue& value = requireField(object, key, context);
    if (!value.isNumber())
        throw std::runtime_error("DataManager: \"" + key + "\" must be a number in " + context);

    const double number = value.number();
    if (number < -static_cast<double>(std::numeric_limits<float>::max()) ||
        number > static_cast<double>(std::numeric_limits<float>::max())) {
        throw std::runtime_error("DataManager: \"" + key + "\" is outside float range in " + context);
    }

    return static_cast<float>(number);
}

std::string requireString(const JsonValue::Object& object,
                          const std::string& key,
                          const std::string& context) {
    const JsonValue& value = requireField(object, key, context);
    if (!value.isString())
        throw std::runtime_error("DataManager: \"" + key + "\" must be a string in " + context);

    return value.string();
}

bool optionalBool(const JsonValue::Object& object,
                  const std::string& key,
                  bool fallback) {
    const auto found = object.find(key);
    if (found == object.end() || !found->second.isBool())
        return fallback;

    return found->second.boolean();
}

std::string optionalString(const JsonValue::Object& object,
                           const std::string& key) {
    const auto found = object.find(key);
    if (found == object.end() || !found->second.isString())
        return "";

    return found->second.string();
}

std::string requireTitle(const JsonValue::Object& object,
                         const std::string& context) {
    const std::string title = optionalString(object, "title");
    if (!title.empty())
        return title;

    return requireString(object, "name", context);
}

LevelConfig parseLevel(const JsonValue& value, std::size_t stageIndex) {
    if (!value.isObject()) {
        throw std::runtime_error("DataManager: stage "
            + std::to_string(stageIndex + 1) + " must be an object");
    }

    const std::string context = "stage " + std::to_string(stageIndex + 1);
    const JsonValue::Object& object = value.object();
    LevelConfig config{
        requireInt(object, "enemyCount", context),
        requireFloat(object, "tankChance", context),
        requireFloat(object, "spawnInterval", context),
        requireFloat(object, "batteryDrain", context),
        requireInt(object, "startAmmo", context)
    };

    return config;
}

ShopAsset parseShopAsset(const JsonValue& value) {
    if (!value.isObject())
        throw std::runtime_error("DataManager: shop asset must be an object");

    const JsonValue::Object& object = value.object();
    const int id = requireInt(object, "id", "shop asset");
    if (id <= 0)
        throw std::runtime_error("DataManager: shop asset id must be positive");

    ShopAssetType type = ShopAssetType::CommandVehicle;
    const std::string typeKey = requireString(object, "type", "shop asset " + std::to_string(id));
    if (!shopAssetTypeFromKey(typeKey, type))
        throw std::runtime_error("DataManager: unknown shop asset type: " + typeKey);

    ShopAsset asset;
    asset.id = id;
    asset.type = type;
    asset.title = requireTitle(object, "shop asset " + std::to_string(id));
    asset.price = requireInt(object, "price", "shop asset " + std::to_string(id));
    asset.file = requireString(object, "file", "shop asset " + std::to_string(id));
    asset.extra = requireInt(object, "extra", "shop asset " + std::to_string(id));
    asset.isDefault = optionalBool(object, "default", false);
    asset.description = optionalString(object, "description");

    if (asset.price < 0)
        throw std::runtime_error("DataManager: shop asset price cannot be negative");

    if (asset.extra < 0)
        throw std::runtime_error("DataManager: shop asset extra cannot be negative");

    if (!std::filesystem::exists(asset.file))
        throw std::runtime_error("DataManager: missing shop asset file: " + asset.file);

    return asset;
}

std::vector<ShopAsset> parseShopAssets(const JsonValue::Object& root) {
    const JsonValue& assetsValue = requireField(root, "shopAssets", "game data root");
    if (!assetsValue.isArray())
        throw std::runtime_error("DataManager: \"shopAssets\" must be an array");

    std::vector<ShopAsset> assets;
    std::set<int> ids;
    std::set<ShopAssetType> defaults;

    for (const JsonValue& value : assetsValue.array()) {
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

bool isHiddenShopCompanionFile(const std::filesystem::path& path) {
    return path.filename().generic_string().starts_with("start-background");
}

int countUncatalogedShopFiles(const std::vector<ShopAsset>& catalog) {
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

const ShopAsset* findDefaultAsset(const std::vector<ShopAsset>& catalog, ShopAssetType type) {
    for (const ShopAsset& asset : catalog) {
        if (asset.type == type && asset.isDefault)
            return &asset;
    }

    return nullptr;
}

void normalizeUserData(UserData& userData, const std::vector<ShopAsset>& catalog) {
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

std::set<int> parseIdArray(const JsonValue::Object& object, const std::string& key) {
    std::set<int> ids;
    const auto found = object.find(key);
    if (found == object.end() || !found->second.isArray())
        return ids;

    for (const JsonValue& value : found->second.array()) {
        if (!value.isNumber())
            continue;

        const double number = value.number();
        if (std::isfinite(number) && std::trunc(number) == number && number > 0.0 &&
            number <= static_cast<double>(std::numeric_limits<int>::max())) {
            ids.insert(static_cast<int>(number));
        }
    }

    return ids;
}

std::string idsToJsonArray(const std::set<int>& ids) {
    std::string result = "[";
    bool first = true;
    for (int id : ids) {
        if (!first)
            result += ", ";

        result += std::to_string(id);
        first = false;
    }
    result += "]";
    return result;
}
}

std::vector<LevelConfig> DataManager::loadLevels(const std::string& path) {
    const JsonValue root = JsonParser(readFile(path)).parse();
    if (!root.isObject())
        throw std::runtime_error("DataManager: level JSON root must be an object");

    const JsonValue& stagesValue = requireField(root.object(), "stages", "level JSON root");
    if (!stagesValue.isArray())
        throw std::runtime_error("DataManager: \"stages\" must be an array");

    std::vector<LevelConfig> stages;
    const JsonValue::Array& stageValues = stagesValue.array();
    stages.reserve(stageValues.size());

    for (std::size_t index = 0; index < stageValues.size(); ++index) {
        LevelConfig config = parseLevel(stageValues[index], index);
        validateLevel(config, index);
        stages.push_back(config);
    }

    if (stages.empty())
        throw std::runtime_error("DataManager: no valid stages found in: " + path);

    return stages;
}

GameData DataManager::loadGameData(const std::string& path) {
    const JsonValue root = JsonParser(readFile(path)).parse();
    if (!root.isObject())
        throw std::runtime_error("DataManager: game data JSON root must be an object");

    GameData gameData;
    const JsonValue& stagesValue = requireField(root.object(), "stages", "game data root");
    if (!stagesValue.isArray())
        throw std::runtime_error("DataManager: \"stages\" must be an array");

    const JsonValue::Array& stageValues = stagesValue.array();
    gameData.stages.reserve(stageValues.size());
    for (std::size_t index = 0; index < stageValues.size(); ++index) {
        LevelConfig config = parseLevel(stageValues[index], index);
        validateLevel(config, index);
        gameData.stages.push_back(config);
    }

    if (gameData.stages.empty())
        throw std::runtime_error("DataManager: no valid stages found in: " + path);

    gameData.shopAssets = parseShopAssets(root.object());
    gameData.uncatalogedShopFiles = countUncatalogedShopFiles(gameData.shopAssets);

    return gameData;
}

UserData DataManager::loadUserData(const std::string& path, const std::vector<ShopAsset>& catalog) {
    UserData userData;

    try {
        const JsonValue root = JsonParser(readFile(path)).parse();
        if (!root.isObject()) {
            normalizeUserData(userData, catalog);
            return userData;
        }

        const JsonValue::Object& rootObject = root.object();
        const auto coins = rootObject.find("coins");
        if (coins != rootObject.end() && coins->second.isNumber()) {
            const double value = coins->second.number();
            if (std::isfinite(value) && std::trunc(value) == value && value >= 0.0 &&
                value <= static_cast<double>(std::numeric_limits<int>::max())) {
                userData.coins = static_cast<int>(value);
            }
        }

        const auto owned = rootObject.find("ownedAssets");
        if (owned != rootObject.end() && owned->second.isObject()) {
            for (ShopAssetType type : allShopAssetTypes()) {
                const std::string key = shopAssetTypeToKey(type);
                userData.ownedAssets[type] = parseIdArray(owned->second.object(), key);
            }
        }

        const auto selected = rootObject.find("selectedAssets");
        if (selected != rootObject.end() && selected->second.isObject()) {
            const JsonValue::Object& selectedObject = selected->second.object();
            for (ShopAssetType type : allShopAssetTypes()) {
                const std::string key = shopAssetTypeToKey(type);
                const auto found = selectedObject.find(key);
                if (found == selectedObject.end() || !found->second.isNumber())
                    continue;

                const double value = found->second.number();
                if (std::isfinite(value) && std::trunc(value) == value && value > 0.0 &&
                    value <= static_cast<double>(std::numeric_limits<int>::max())) {
                    userData.selectedAssets[type] = static_cast<int>(value);
                }
            }
        }
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

    file << "{\n";
    file << "  \"coins\": " << std::max(0, userData.coins) << ",\n";
    file << "  \"ownedAssets\": {\n";

    const std::vector<ShopAssetType> types = allShopAssetTypes();
    for (std::size_t index = 0; index < types.size(); ++index) {
        const ShopAssetType type = types[index];
        const auto found = userData.ownedAssets.find(type);
        const std::set<int> empty;
        const std::set<int>& ids = found == userData.ownedAssets.end() ? empty : found->second;
        file << "    \"" << shopAssetTypeToKey(type) << "\": " << idsToJsonArray(ids);
        file << (index + 1 < types.size() ? "," : "") << "\n";
    }

    file << "  },\n";
    file << "  \"selectedAssets\": {\n";

    for (std::size_t index = 0; index < types.size(); ++index) {
        const ShopAssetType type = types[index];
        const auto found = userData.selectedAssets.find(type);
        const int id = found == userData.selectedAssets.end() ? 0 : found->second;
        file << "    \"" << shopAssetTypeToKey(type) << "\": " << id;
        file << (index + 1 < types.size() ? "," : "") << "\n";
    }

    file << "  }\n";
    file << "}\n";
}

int DataManager::loadCoins(const std::string& path) {
    try {
        const JsonValue root = JsonParser(readFile(path)).parse();
        if (!root.isObject())
            return 0;

        const auto found = root.object().find("coins");
        if (found == root.object().end() || !found->second.isNumber())
            return 0;

        const double coins = found->second.number();
        if (!std::isfinite(coins) || std::trunc(coins) != coins)
            return 0;

        if (coins < 0.0)
            return 0;

        if (coins > static_cast<double>(std::numeric_limits<int>::max()))
            return std::numeric_limits<int>::max();

        return static_cast<int>(coins);
    } catch (const std::exception&) {
        return 0;
    }
}

void DataManager::saveCoins(const std::string& path, int coins) {
    std::ofstream file(path);
    if (!file)
        return;

    file << "{\n"
         << "  \"coins\": " << std::max(0, coins) << "\n"
         << "}\n";
}

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
