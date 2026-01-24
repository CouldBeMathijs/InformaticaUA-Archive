#include "ResourceManager.h"
#include "../../../DesignByContract.h"
#include "../../../ErrorMessages.h"

#include <fstream>
#include <iostream>

ResourceManager::ResourceManager() {
    _initCheck = this;
    ENSURE(properlyInitialized(),
           ErrorMessagesInstructions::properInitializationEnsure("ResourceManager"));
}

bool ResourceManager::properlyInitialized() const {
    return _initCheck == this;
}

void ResourceManager::loadAllTextures() {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("ResourceManager"));

    std::vector<std::pair<std::string, std::string>> texturesToLoad = {
        {"car", "resources/sprites/vehicles/cars/car1.png"},
        {"car", "resources/sprites/vehicles/cars/car1.png"},
        {"road", "resources/sprites/roads/road.png"},
        {"intersection", "resources/sprites/roads/intersection.png"},
        //{"bus_stop", "resources/sprites/stops/bus_stop.png"},
        //{"traffic_light", "resources/sprites/stops/traffic_light.png"}
    };

    for (const auto& [id, path] : texturesToLoad) {
        if (!loadTexture(id, path)) {
            std::cerr << "Failed to load texture: " << path << std::endl;
        }
    }
}

bool ResourceManager::loadTexture(const std::string& id, const std::string& path) {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("ResourceManager"));
    REQUIRE(!id.empty(),   ErrorMessagesInstructions::missingParameter("id"));
    REQUIRE(!path.empty(), ErrorMessagesInstructions::missingParameter("path"));

    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }
    textures[id] = texture;

    ENSURE(getTexture(id) != nullptr, ErrorMessagesInstructions::notUpdatedSuccessfully("texture"));

    return true;
}

sf::Texture* ResourceManager::getTexture(const std::string& id) {
    REQUIRE(!id.empty(), ErrorMessagesInstructions::missingParameter("id"));

    auto it = textures.find(id);
    if (it != textures.end()) {
        return &it->second;
    }
    return nullptr;
}

bool ResourceManager::loadFont(const std::string& id, const std::string& path) {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("ResourceManager"));
    REQUIRE(!id.empty(),   ErrorMessagesInstructions::missingParameter("id"));
    REQUIRE(!path.empty(), ErrorMessagesInstructions::missingParameter("path"));

    sf::Font font;
    if (!font.loadFromFile(path)) {
        std::cerr << "Failed to load font: " << path << std::endl;
        return false;
    }
    fonts[id] = font;

    ENSURE(getFont(id) != nullptr, ErrorMessagesInstructions::notUpdatedSuccessfully("font"));
    return true;
}

sf::Font* ResourceManager::getFont(const std::string& id) {
    REQUIRE(!id.empty(), ErrorMessagesInstructions::missingParameter("id"));

    auto it = fonts.find(id);
    if (it != fonts.end()) {
        return &it->second;
    }
    return nullptr;
}

void ResourceManager::clear() {
    REQUIRE(properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("ResourceManager"));

    textures.clear();
    fonts.clear();

    ENSURE(textures.empty() && fonts.empty(), ErrorMessagesInstructions::notDeletedSuccessfully("resources"));
}