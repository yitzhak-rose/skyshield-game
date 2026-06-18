#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <functional>
#include <stdexcept>
#include "Enums/Fonts.h"
#include "Enums/GameObjects.h"
#include "Resources.h"

/// @brief Singleton class that manages the loading, storing, and retrieval of game resources.
class ResourceManager {
public:
    /// @brief Gets the singleton instance of the ResourceManager.
    /// @return Reference to the singleton ResourceManager instance.
    static ResourceManager& instance();

    /// @brief Gets all loaded game object textures.
    /// @return Reference to the map of GameObject enums to sf::Texture.
    std::map<GameObjects, sf::Texture>& getImages();

    /// @brief Retrieves a specific texture by its GameObject enum key.
    /// @param image The GameObject enum representing the desired texture.
    /// @return Reference to the requested sf::Texture.
    sf::Texture& getImage(GameObjects image);

    /// @brief Retrieves a specific font by its font enum key.
    /// @param font The font enum representing the desired font.
    /// @return Reference to the requested sf::Font.
    sf::Font& getFont(Fonts font);

private:
    /// @brief Private constructor to enforce the singleton pattern.
    /// @throws std::runtime_error If any resource fails to load.
    ResourceManager();

    /// @brief Default destructor.
    ~ResourceManager() = default;

    // Delete copy/move operations to enforce singleton behavior.
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /// @brief Loads all resource configurations defined in the Resources arrays.
    /// @throws std::runtime_error If any resource loading fails.
    void loadData();

    /// @brief Template helper function to load a list of resource infos into a destination map.
    /// @tparam Key The key type for the map (e.g. enum class).
    /// @tparam Value The value type to store (e.g. sf::Texture).
    /// @tparam N The size of the resources array.
    /// @param resources The statically defined list of resources to load.
    /// @param dataMap The target map to populate.
    /// @param pathResolver An optional callback function to resolve dynamic paths.
    /// @throws std::runtime_error If any file fails to load.
    template<typename Key, typename Value, std::size_t N>
    void loadResources(const std::array<ResourceInfo<Key>, N>& resources, 
                        std::map<Key, Value>& dataMap,
                        std::function<std::string(Key)> pathResolver = nullptr);

    /// @brief Storage for loaded game object textures.
    std::map<GameObjects, sf::Texture> m_imageTextures;

    /// @brief Storage for loaded fonts.
    std::map<Fonts, sf::Font> m_fonts;
};
