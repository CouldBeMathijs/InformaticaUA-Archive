#ifndef PACMAN_DIRECTION_H
#define PACMAN_DIRECTION_H
#include "../Position.h"

namespace Direction {
/**
 * @brief The order of the Directions is the same as the order in the Spritemap
 */
enum class Cardinal {
    EAST,
    SOUTH,
    WEST,
    NORTH,
    NONE,
};

constexpr Position getValue(const Cardinal e) {
    switch (e) {
    case Cardinal::EAST:
        return {1, 0};
    case Cardinal::NORTH:
        return {0, -1};
    case Cardinal::SOUTH:
        return {0, 1};
    case Cardinal::WEST:
        return {-1, 0};
    default:
        break;
    }
    return {0, 0};
}

inline const char* to_string(const Cardinal e) {
    switch (e) {
    case Cardinal::EAST:
        return "EAST";
    case Cardinal::SOUTH:
        return "SOUTH";
    case Cardinal::WEST:
        return "WEST";
    case Cardinal::NORTH:
        return "NORTH";
    case Cardinal::NONE:
        return "NONE";
    default:
        return "unknown";
    }
}

/**
 * @brief Checks if two given directions are exact opposites.
 *
 * @param d1 The first direction.
 * @param d2 The second direction.
 * @return true if d1 is the opposite of d2.
 */
constexpr bool isOpposite(const Cardinal d1, const Cardinal d2) {
    if (d1 == d2)
        return false;

    switch (d1) {
    case Cardinal::EAST:
        return d2 == Cardinal::WEST;
    case Cardinal::WEST:
        return d2 == Cardinal::EAST;
    case Cardinal::NORTH:
        return d2 == Cardinal::SOUTH;
    case Cardinal::SOUTH:
        return d2 == Cardinal::NORTH;
    case Cardinal::NONE:
        return false;
    }
    return false;
}

/**
 * @param d A direction
 * @return The opposite of d
 */
constexpr Cardinal getOpposite(const Cardinal d) {
    switch (d) {
    case Cardinal::EAST:
        return Cardinal::WEST;
    case Cardinal::SOUTH:
        return Cardinal::NORTH;
    case Cardinal::WEST:
        return Cardinal::EAST;
    case Cardinal::NORTH:
        return Cardinal::SOUTH;
    case Cardinal::NONE:
        break;
    }
    return Cardinal::NONE;
}
} // namespace Direction
#endif // PACMAN_DIRECTION_H