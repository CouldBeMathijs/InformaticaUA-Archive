#ifndef PACMAN_LOGIC_CONSTANTS_H
#define PACMAN_LOGIC_CONSTANTS_H

#include <algorithm>
#include <cmath>

namespace LogicConstants {
constexpr unsigned int AMOUNT_OF_TILES_WIDTH = 20;
constexpr double TILE_WIDTH = 1.0 / AMOUNT_OF_TILES_WIDTH;
constexpr unsigned int AMOUNT_OF_TILES_HEIGHT = 11;
constexpr double TILE_HEIGHT = 1.0 / AMOUNT_OF_TILES_HEIGHT;
constexpr float TARGET_ASPECT_RATIO =
    static_cast<float>(AMOUNT_OF_TILES_WIDTH) / static_cast<float>(AMOUNT_OF_TILES_HEIGHT);
constexpr float REVERSE_TARGET_ASPECT_RATIO = 1 / TARGET_ASPECT_RATIO;
constexpr double BASE_SPEED = 3.0 / std::min(AMOUNT_OF_TILES_HEIGHT, AMOUNT_OF_TILES_WIDTH);
constexpr double ANIMATION_SPEED = 0.12;
} // namespace LogicConstants
#endif
