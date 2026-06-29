#include "Utilities/AppUtils.h"

#include "App/Constants.h"

#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>

bool AppUtils::sameUserData(const UserData& lhs, const UserData& rhs) {
    return lhs.coins == rhs.coins &&
        lhs.muted == rhs.muted &&
        sameFloat(lhs.maxVolume, rhs.maxVolume) &&
        lhs.ownedAssets == rhs.ownedAssets &&
        lhs.selectedAssets == rhs.selectedAssets;
}

bool AppUtils::sameFloat(float lhs, float rhs) {
    return std::abs(lhs - rhs) <= 0.001f;
}

sf::Vector2u AppUtils::initialWindowSize() {
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    return {
        std::min(static_cast<unsigned int>(WindowConfig::Width), desktopMode.size.x),
        std::min(static_cast<unsigned int>(WindowConfig::Height), desktopMode.size.y)
    };
}

std::string AppUtils::matchingStartBackgroundPath(const std::string& backgroundPath) {
    const std::filesystem::path path(backgroundPath);
    const std::filesystem::path startFilename = "start-" + path.filename().string();
    return (path.parent_path() / startFilename).generic_string();
}
