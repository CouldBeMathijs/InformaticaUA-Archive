#ifndef PACMAN_SPRITEMAP_H
#define PACMAN_SPRITEMAP_H

#include <SFML/Graphics.hpp>

namespace Assets {
using TextureLocation = sf::IntRect;

/**
 * @brief Switch between all base sprite locations
 */
enum class SpriteDefinition {
    CoinBase,
    FruitBase,
    GhostBlueBase,
    GhostOrangeBase,
    GhostPanicBase,
    GhostPinkBase,
    GhostRedBase,
    PacmanBase,
    WallBase,
    PacmanDeathBase,
    GhostDeathBase,
    SpawnWallBase,
    GhostPanicAlternate,
};

/**
 * @return The requested location on the Spritemap
 */
TextureLocation getSpriteInfo(SpriteDefinition);

/**
 * @return The texture of the spritemap
 */
const sf::Texture& getSpritemapTexture();

/**
 * @return The font used for all text in the game
 */
const sf::Font& getDefaultFont();

} // namespace Assets

#endif // PACMAN_SPRITEMAP_H