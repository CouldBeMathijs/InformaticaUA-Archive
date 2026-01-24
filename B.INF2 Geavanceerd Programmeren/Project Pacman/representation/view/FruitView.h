/**
 * @file FruitView.h
 * @brief Visual representation for bonus fruit entities.
 */

#ifndef PACMAN_FRUIT_VIEW_H
#define PACMAN_FRUIT_VIEW_H

#include "IEntityView.h"

/**
 * @class FruitView
 * @brief The graphical "View" for bonus items (fruit).
 * * Inherits from IEntityView to receive coordinate updates and
 * handle rendering. Unlike coins, fruit often has a limited
 * lifespan on the screen, which is managed by the coupled model.
 */
class FruitView final : public IEntityView {
public:
    /**
     * @brief Constructs a FruitView and binds it to a logical fruit entity.
     * * The constructor initializes the specific fruit sprite (e.g., Cherry,
     * Orange, or Melon) based on the current level or model data.
     * @param model A shared pointer to the logical IEntityModel representing the fruit.
     */
    explicit FruitView(std::shared_ptr<IEntityModel> model);
};

#endif // PACMAN_FRUIT_VIEW_H