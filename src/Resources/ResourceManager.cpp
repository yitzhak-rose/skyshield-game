#include "Resources/ResourceManager.h"
#include "Utilities/AppUtils.h"

#include <fstream>
#include <stdexcept>
#include <type_traits>

#pragma region Public
ResourceManager& ResourceManager::instance() {
    static ResourceManager _instance;
    return _instance;
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
            case ShopAssetType::FastEnemy:
                setImage(GameObjects::SmallEnemy, asset.file);
                break;
            case ShopAssetType::TankEnemy:
                setImage(GameObjects::Enemy, asset.file);
                break;
            case ShopAssetType::Background:
                setImage(GameObjects::Background, asset.file);
                setImage(GameObjects::StartBackground, AppUtils::matchingStartBackgroundPath(asset.file));
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
#pragma endregion

#pragma region Resource Loading
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
    for (const auto& resource : resources) {
        Value value;
        const std::string path = resolveResourcePath(resource, pathResolver);
        loadResourceValue(value, path);
        storeResource(resource.type, value, dataMap);
    }
}

template<typename Key>
std::string ResourceManager::resolveResourcePath(const ResourceInfo<Key>& resource,
                                                 const std::function<std::string(Key)>& pathResolver) {
    if (!pathResolver)
        return resource.path;

    const std::string resolvedPath = pathResolver(resource.type);
    return resolvedPath.empty() ? resource.path : resolvedPath;
}

template<typename Value>
void ResourceManager::loadResourceValue(Value& value, const std::string& path) {
    bool loaded = false;
    if constexpr (std::is_same_v<Value, sf::Font>)
        loaded = value.openFromFile(path);
    else
        loaded = value.loadFromFile(path);

    if (!loaded)
        throw std::runtime_error("Failed to load resource from path: " + path);
}

template<typename Key, typename Value>
void ResourceManager::storeResource(Key type, Value& value, std::map<Key, Value>& dataMap) {
    if (type != Key::Max)
        dataMap[type] = value;
}

void ResourceManager::loadSoundPaths() {
    for (const auto& resource : Resources::soundResources) {
        validateSoundPath(resource.path);

        if (resource.type != Sounds::Max)
            m_soundPaths[resource.type] = resource.path;
    }
}

void ResourceManager::validateSoundPath(const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Failed to find sound resource from path: " + path);
}
#pragma endregion

#pragma region Utilities
void ResourceManager::setImage(GameObjects image, const std::string& path) {
    m_imageTextures[image] = loadTextureFromFile(path);
}

void ResourceManager::setSoundPath(Sounds sound, const std::string& path) {
    validateSoundPath(path);
    m_soundPaths[sound] = path;
}

void ResourceManager::setFont(Fonts font, const std::string& path) {
    m_fonts[font] = loadFontFromFile(path);
}
#pragma endregion
