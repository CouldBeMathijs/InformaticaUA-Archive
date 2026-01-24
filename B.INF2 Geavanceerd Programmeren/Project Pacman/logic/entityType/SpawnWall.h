#ifndef PACMAN_SPAWN_WALL_H
#define PACMAN_SPAWN_WALL_H
#include "IEntityModel.h"

class SpawnWall final : public IEntityModel {
public:
    explicit SpawnWall(const Rectangle& hitBox);
    /**
     * @brief Calls the correct double dispatch
     * @param visitor Singularly dispatched visitor object
     */
    void accept(IEntityVisitor& visitor) override;
};

#endif
