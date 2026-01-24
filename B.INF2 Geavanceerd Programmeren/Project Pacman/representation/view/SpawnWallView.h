/**
 * @file SpawnWallView.h
 * @brief Visual representation for the ghost spawn area boundaries.
 */

#ifndef PACMAN_SPAWN_WALL_VIEW_H
#define PACMAN_SPAWN_WALL_VIEW_H

#include "IEntityView.h"

/**
 * @class SpawnWallView
 * @brief Graphical view for walls associated with the ghost respawn zone.
 */
class SpawnWallView final : public IEntityView {
public:
    /**
     * @brief Constructs a SpawnWallView and links it to a logical wall model.
     * * The constructor initializes the specific texture or color for the
     * spawn gate, ensuring it is visually distinguishable from standard
     * environmental walls.
     * * @param model A shared pointer to the logical entity representing this wall.
     */
    explicit SpawnWallView(const std::shared_ptr<IEntityModel>& model);
};

#endif // PACMAN_SPAWN_WALL_VIEW_H