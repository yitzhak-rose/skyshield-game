#pragma once

#include <nlohmann/json.hpp>

#include <string>

/// @brief Generic helpers for reading JSON files and validating JSON fields.
namespace JsonUtils {
/// @brief Short project-local alias for the vendored nlohmann JSON type.
using Json = nlohmann::json;

/// @brief Reads and parses a JSON file.
/// @param path Path to the JSON file.
/// @return Parsed JSON value.
/// @throws std::runtime_error if the file cannot be opened or parsed.
Json readJsonFile(const std::string& path);

/// @brief Finds a required field in a JSON object.
/// @param object JSON object to inspect.
/// @param key Field name to require.
/// @param context Human-readable location used in error messages.
/// @return Reference to the found field value.
/// @throws std::runtime_error if object is not an object or the field is missing.
const Json& requireField(const Json& object, const std::string& key, const std::string& context);

/// @brief Reads a required integer field from a JSON object.
/// @param object JSON object to inspect.
/// @param key Field name to require.
/// @param context Human-readable location used in error messages.
/// @return Field value as int.
/// @throws std::runtime_error if the field is missing, not an integer, or outside int range.
int requireInt(const Json& object, const std::string& key, const std::string& context);

/// @brief Reads a required floating-point field from a JSON object.
/// @param object JSON object to inspect.
/// @param key Field name to require.
/// @param context Human-readable location used in error messages.
/// @return Field value as float.
/// @throws std::runtime_error if the field is missing, not numeric, or outside float range.
float requireFloat(const Json& object, const std::string& key, const std::string& context);

/// @brief Reads a required string field from a JSON object.
/// @param object JSON object to inspect.
/// @param key Field name to require.
/// @param context Human-readable location used in error messages.
/// @return Field value as std::string.
/// @throws std::runtime_error if the field is missing or not a string.
std::string requireString(const Json& object, const std::string& key, const std::string& context);

/// @brief Reads an optional string field from a JSON object.
/// @param object JSON object to inspect.
/// @param key Field name to read.
/// @return Field value, or an empty string when missing or not a string.
std::string optionalString(const Json& object, const std::string& key);

/// @brief Reads an optional boolean field from a JSON object.
/// @param object JSON object to inspect.
/// @param key Field name to read.
/// @param fallback Value to return when the field is missing or not a boolean.
/// @return Field value, or fallback when missing or invalid.
bool optionalBool(const Json& object, const std::string& key, bool fallback);
}
