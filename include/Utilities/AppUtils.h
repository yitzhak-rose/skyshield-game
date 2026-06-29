#pragma once

#include "Structs/UserData.h"

#include <SFML/System/Vector2.hpp>
#include <string>

namespace AppUtils {
    /// @brief Returns true when two user-data snapshots are equivalent.
    bool sameUserData(const UserData& lhs, const UserData& rhs);

    /// @brief Clamps the initial window size to the current desktop mode.
    sf::Vector2u initialWindowSize();

    /// @brief Returns the matching start-screen background path for a game background.
    std::string matchingStartBackgroundPath(const std::string& backgroundPath);
}
