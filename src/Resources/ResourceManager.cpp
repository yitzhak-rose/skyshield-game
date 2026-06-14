#include "Resources/ResourceManager.h"
#include <iostream>
#include <stdexcept>

ResourceManager& ResourceManager::instance() {
    static ResourceManager _instance;
    return _instance;
}

ResourceManager::ResourceManager() {
    loadData();
}

void ResourceManager::loadData() {
    loadResources(Resources::imageResources, m_imageTextures);
}

std::map<GameObjects, sf::Texture>& ResourceManager::getImages() {
    return m_imageTextures;
}

sf::Texture& ResourceManager::getImage(GameObjects image) {
    return m_imageTextures[image];
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

        if (!val.loadFromFile(path)) {
            throw std::runtime_error("Failed to load resource from path: " + path);
        }

        if (type != Key::Max) {
            dataMap[type] = val;
        }
    }
}
