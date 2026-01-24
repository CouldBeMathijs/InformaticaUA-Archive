#include "AssetManager.h"

#include <array>

// Anonymous namespace to make sure the values are only accessed after initialization
namespace {
sf::Texture k_texture;
sf::Font k_font;

bool initialized = false;

using namespace Assets;

constexpr size_t to_underlying(const SpriteDefinition e) noexcept { return static_cast<size_t>(e); }

const std::array k_spriteDefinitions = {
    TextureLocation{414, 266, 10, 10}, // SpriteDefinition::Coin
    TextureLocation{594, -4, 50, 50},  // SpriteDefinition::Fruit
    TextureLocation{744, -4, 50, 50},  // SpriteDefinition::GhostBlue
    TextureLocation{794, -4, 50, 50},  // SpriteDefinition::GhostOrange
    TextureLocation{-6, 547, 50, 50},  // SpriteDefinition::GhostPanic
    TextureLocation{694, -4, 50, 50},  // SpriteDefinition::GhostPink
    TextureLocation{644, -4, 50, 50},  // SpriteDefinition::GhostRed
    TextureLocation{844, -4, 50, 50},  // SpriteDefinition::Pacman
    TextureLocation{644, 395, 50, 50}, // SpriteDefinition::Wall
    TextureLocation{344, -4, 50, 50},  // SpriteDefinition::PacmanDeath
    TextureLocation{244, -4, 50, 50},  // SpriteDefinition::GhostDeathBase
    TextureLocation{-1, -1, 1, 1},     // SpriteDefinition::SpawnWall
    TextureLocation{44, 547, 50, 50},  // SpriteDefinition::GhostPanicAlternate
};

void initialize() {
    if (initialized) {
        return;
    }

    if (!k_texture.loadFromFile("assets/spritemap.png")) {
        throw std::invalid_argument("The spritemap is not at the expected location of \"assets/spritemap.png\"");
    }

    if (!k_font.loadFromFile("./assets/BoldPixels/BoldPixels.ttf")) {
        throw std::runtime_error("Missing font BoldPixels");
    }

    initialized = true;
}

} // namespace

TextureLocation Assets::getSpriteInfo(const SpriteDefinition d) { return k_spriteDefinitions[to_underlying(d)]; }

const sf::Texture& Assets::getSpritemapTexture() {
    initialize();
    return k_texture;
}

const sf::Font& Assets::getDefaultFont() {
    initialize();
    return k_font;
}