#include "Resources/ResourceManager.h"
#include <iostream>
#include <stdexcept>
#include <type_traits>

ResourceManager& ResourceManager::instance() {
    static ResourceManager _instance;
    return _instance;
}

ResourceManager::ResourceManager() {
    loadData();
}

void ResourceManager::loadData() {
    loadResources(Resources::imageResources, m_imageTextures);
    loadResources(Resources::fontResources, m_fonts);
}

std::map<GameObjects, sf::Texture>& ResourceManager::getImages() {
    return m_imageTextures;
}

sf::Texture& ResourceManager::getImage(GameObjects image) {
    auto it = m_imageTextures.find(image);
    if (it == m_imageTextures.end())
        throw std::runtime_error("Requested texture is not loaded (id "
            + std::to_string(static_cast<int>(image)) + ")");
    return it->second;
}

sf::Font& ResourceManager::getFont(Fonts font) {
    auto it = m_fonts.find(font);
    if (it == m_fonts.end())
        throw std::runtime_error("Requested font is not loaded (id "
            + std::to_string(static_cast<int>(font)) + ")");
    return it->second;
}

// PRIVATE

template<typename Key, typename Value, std::size_t N>
void ResourceManager::loadResources(const std::array<ResourceInfo<Key>, N>& resources, 
                                    std::map<Key, Value>& dataMap,
                                    std::function<std::string(Key)> pathResolver) {
    const std::size_t count = resources.size();
    for (std::size_t i = 0; i < count; i++) {
        Value val;
        auto resource = resources[i];
        Key type = resource.type;
        
        std::string path;
        if (pathResolver) {
            path = pathResolver(type);
            // Fallback to resource.path if resolver returns empty
            if (path.empty()) path = resource.path;
        } else {
            path = resource.path;
        }

        bool loaded = false;
        if constexpr (std::is_same_v<Value, sf::Font>) {
            loaded = val.openFromFile(path);
        }
        else {
            loaded = val.loadFromFile(path);
        }

        if (!loaded) {
            throw std::runtime_error("Failed to load resource from path: " + path);
        }

        if (type != Key::Max) {
            dataMap[type] = val;
        }
    }
}
