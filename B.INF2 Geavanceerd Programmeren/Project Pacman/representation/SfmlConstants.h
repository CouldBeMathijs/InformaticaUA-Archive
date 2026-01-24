#ifndef PACMAN_SFML_CONSTANTS_H
#define PACMAN_SFML_CONSTANTS_H
#include "LogicConstants.h"

namespace SfmlConstants {
constexpr float VIEW_HEIGHT = 800.0f; // A constant height for the game world
constexpr float VIEW_WIDTH = VIEW_HEIGHT * LogicConstants::TARGET_ASPECT_RATIO;
} // namespace SfmlConstants
#endif
