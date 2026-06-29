#pragma once

#include <string>

/// @brief Generic metadata that maps a resource key to its default file path.
template <typename Key>
struct ResourceInfo {
    Key type;
    std::string path;
};
