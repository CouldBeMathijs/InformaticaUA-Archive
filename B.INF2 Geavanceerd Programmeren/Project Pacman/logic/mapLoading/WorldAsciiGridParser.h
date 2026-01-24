#ifndef PACMAN_WORLD_ASCII_GRID_PARSER_H
#define PACMAN_WORLD_ASCII_GRID_PARSER_H
#include "IWorldConfigParser.h"

#include <fstream>

/**
 * @brief A parser to a worldGrid based on a simple ascii representation of the world
 */
class WorldAsciiGridParser final : public IWorldConfigParser {
public:
    /**
     * @param filename The file to parse
     * @return The type WorldCreator can use to generate the world
     */
    [[nodiscard]] std::vector<std::vector<char>> parse(const std::string& filename) const override;
};

#endif
