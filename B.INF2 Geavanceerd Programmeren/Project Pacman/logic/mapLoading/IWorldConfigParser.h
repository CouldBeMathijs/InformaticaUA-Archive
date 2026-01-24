#ifndef PACMAN_I_WORLD_CONFIG_PARSER_H
#define PACMAN_I_WORLD_CONFIG_PARSER_H
#include <string>
#include <vector>

/**
 * @brief Pure virtual class on which various to_worldGrid parsers can be based
 */
class IWorldConfigParser {
public:
    virtual ~IWorldConfigParser() = default;
    [[nodiscard]] virtual std::vector<std::vector<char>> parse(const std::string& filename) const = 0;
};
#endif
