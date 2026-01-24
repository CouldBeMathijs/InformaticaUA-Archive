#ifndef PACMAN_WORLD_CREATOR_H
#define PACMAN_WORLD_CREATOR_H
#include "../World.h"

class IEntityFactory;
class IWorldConfigParser;

/**
 * @brief Purely static class, allows for generating the world_object using different IWorldConfigParsers
 */
class WorldCreator {
public:
    /**
     * @brief Static method for creating a World
     * @param filename The filename to read in
     * @param parser The parser to use
     * @param factory The factory to use to populate the world
     * @return A fully populated world
     */
    static World createWorld(const std::string& filename, const IWorldConfigParser& parser,
                             const std::shared_ptr<IEntityFactory>& factory);
};

#endif
