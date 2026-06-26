#include "Resources/ResourceManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace {
std::string matchingStartBackgroundPath(const std::string& backgroundPath) {
    const std::filesystem::path path(backgroundPath);
    const std::filesystem::path startFilename = "start-" + path.filename().string();
    return (path.parent_path() / startFilename).generic_string();
}
}

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
    loadSoundPaths();
}

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

void ResourceManager::loadSoundPaths() {
    for (const auto& resource : Resources::soundResources) {
        std::ifstream file(resource.path);
        if (!file)
            throw std::runtime_error("Failed to find sound resource from path: " + resource.path);

        if (resource.type != Sounds::Max)
            m_soundPaths[resource.type] = resource.path;
    }
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

const std::string& ResourceManager::getSoundPath(Sounds sound) const {
    auto it = m_soundPaths.find(sound);
    if (it == m_soundPaths.end())
        throw std::runtime_error("Requested sound is not loaded (id "
            + std::to_string(static_cast<int>(sound)) + ")");
    return it->second;
}

void ResourceManager::applyShopSelections(const std::vector<ShopAsset>& catalog, const UserData& userData) {
    for (const ShopAsset& asset : catalog) {
        const auto selected = userData.selectedAssets.find(asset.type);
        if (selected == userData.selectedAssets.end() || selected->second != asset.id)
            continue;

        switch (asset.type) {
            case ShopAssetType::CommandVehicle:
                setImage(GameObjects::CommandCar, asset.file);
                break;
            case ShopAssetType::Drone:
                setImage(GameObjects::Drone, asset.file);
                break;
            case ShopAssetType::Background:
                setImage(GameObjects::Background, asset.file);
                setImage(GameObjects::StartBackground, matchingStartBackgroundPath(asset.file));
                break;
            case ShopAssetType::BackgroundMusic:
                setSoundPath(Sounds::Background, asset.file);
                break;
            case ShopAssetType::Font:
                setFont(Fonts::StencilForce, asset.file);
                break;
        }
    }
}

sf::Texture ResourceManager::loadTextureFromFile(const std::string& path) const {
    sf::Texture texture;
    if (!texture.loadFromFile(path))
        throw std::runtime_error("Failed to load texture from path: " + path);

    return texture;
}

sf::Font ResourceManager::loadFontFromFile(const std::string& path) const {
    sf::Font font;
    if (!font.openFromFile(path))
        throw std::runtime_error("Failed to load font from path: " + path);

    return font;
}

void ResourceManager::setImage(GameObjects image, const std::string& path) {
    m_imageTextures[image] = loadTextureFromFile(path);
}

void ResourceManager::setSoundPath(Sounds sound, const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Failed to find sound resource from path: " + path);

    m_soundPaths[sound] = path;
}

void ResourceManager::setFont(Fonts font, const std::string& path) {
    m_fonts[font] = loadFontFromFile(path);
}
