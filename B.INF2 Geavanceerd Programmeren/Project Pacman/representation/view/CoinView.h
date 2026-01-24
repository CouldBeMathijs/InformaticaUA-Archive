/**
 * @file CoinView.h
 * @brief Visual representation for collectable coins (pellets).
 */

#ifndef PACMAN_COIN_VIEW_H
#define PACMAN_COIN_VIEW_H

#include "IEntityView.h"

/**
 * @class CoinView
 * @brief The graphical "View" for a coin entity.
 * * This class handles the rendering logic for coins. It observes a
 * corresponding IEntityModel and ensures the visual sprite matches
 * the logical position in the World.
 */
class CoinView final : public IEntityView {
public:
    /**
     * @brief Constructs a CoinView and binds it to a logical entity.
     * * The constructor typically initializes the coin's sprite,
     * sets its texture or color, and performs the initial alignment
     * to the model's position.
     * * @param model A shared pointer to the logic model this view represents.
     */
    explicit CoinView(std::shared_ptr<IEntityModel> model);
};

#endif // PACMAN_COIN_VIEW_H