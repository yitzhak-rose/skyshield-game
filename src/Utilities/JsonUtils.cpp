#include "Utilities/JsonUtils.h"

#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace JsonUtils {
Json readJsonFile(const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("JsonUtils: cannot open JSON file: " + path);

    try {
        return Json::parse(file);
    } catch (const Json::parse_error& error) {
        throw std::runtime_error("JsonUtils: JSON parse error in " + path + ": " + error.what());
    }
}

const Json& requireField(const Json& object, const std::string& key, const std::string& context) {
    if (!object.is_object())
        throw std::runtime_error("JsonUtils: expected object in " + context);

    const auto found = object.find(key);
    if (found == object.end())
        throw std::runtime_error("JsonUtils: missing \"" + key + "\" in " + context);

    return *found;
}

int requireInt(const Json& object, const std::string& key, const std::string& context) {
    const Json& value = requireField(object, key, context);
    if (!value.is_number_integer())
        throw std::runtime_error("JsonUtils: \"" + key + "\" must be an integer in " + context);

    const long long number = value.get<long long>();
    if (number < static_cast<long long>(std::numeric_limits<int>::min()) ||
        number > static_cast<long long>(std::numeric_limits<int>::max())) {
        throw std::runtime_error("JsonUtils: \"" + key + "\" is outside integer range in " + context);
    }

    return static_cast<int>(number);
}

float requireFloat(const Json& object, const std::string& key, const std::string& context) {
    const Json& value = requireField(object, key, context);
    if (!value.is_number())
        throw std::runtime_error("JsonUtils: \"" + key + "\" must be a number in " + context);

    const double number = value.get<double>();
    if (!std::isfinite(number) ||
        number < -static_cast<double>(std::numeric_limits<float>::max()) ||
        number > static_cast<double>(std::numeric_limits<float>::max())) {
        throw std::runtime_error("JsonUtils: \"" + key + "\" is outside float range in " + context);
    }

    return static_cast<float>(number);
}

std::string requireString(const Json& object, const std::string& key, const std::string& context) {
    const Json& value = requireField(object, key, context);
    if (!value.is_string())
        throw std::runtime_error("JsonUtils: \"" + key + "\" must be a string in " + context);

    return value.get<std::string>();
}

std::string optionalString(const Json& object, const std::string& key) {
    const auto found = object.find(key);
    if (found == object.end() || !found->is_string())
        return "";

    return found->get<std::string>();
}

bool optionalBool(const Json& object, const std::string& key, bool fallback) {
    const auto found = object.find(key);
    if (found == object.end() || !found->is_boolean())
        return fallback;

    return found->get<bool>();
}
}
