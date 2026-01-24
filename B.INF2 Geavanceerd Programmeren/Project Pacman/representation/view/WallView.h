/**
 * @file WallView.h
 * @brief Visual representation for the standard maze walls.
 */

#ifndef PACMAN_WALL_VIEW_H
#define PACMAN_WALL_VIEW_H

#include "IEntityView.h"

/**
 * @class WallView
 * @brief Graphical "View" for static environment obstacles.
 */
class WallView final : public IEntityView {
public:
    /**
     * @brief Constructs a WallView and binds it to a structural model.
     * @param model A shared pointer to the logical wall entity.
     */
    explicit WallView(std::shared_ptr<IEntityModel> model);
};

#endif // PACMAN_WALL_VIEW_H