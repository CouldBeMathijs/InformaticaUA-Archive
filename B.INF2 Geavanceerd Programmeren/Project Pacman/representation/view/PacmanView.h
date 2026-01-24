/**
 * @file PacmanView.h
 * @brief Visual representation and animation logic for the Pac-Man character.
 */

#ifndef PACMAN_PACMAN_VIEW_H
#define PACMAN_PACMAN_VIEW_H

#include "IDirectionalEntityView.h"

/**
 * @class PacmanView
 * @brief Handles the rendering and animation of the player entity.
 * * This class uses directional offsets to ensure Pac-Man faces the direction
 * of travel and manages the timing of the mouth-opening animation.
 */
class PacmanView final : public IDirectionalEntityView {
    bool m_dying = false;

public:
    /**
     * @brief Constructs a PacmanView and binds it to the player model.
     * * Initializes the spritesheet coordinates and sets the animation
     * parameters (frames and speed).
     * @param model A shared pointer to the logical Pacman model.
     */
    explicit PacmanView(std::shared_ptr<IEntityModel> model);

    /**
     * @brief Updates the sprite's position, rotation, and animation frame.
     * * Checks the current direction from the model and adjusts the
     * texture rectangle accordingly. It also processes the mouth
     * "ping-pong" animation (Open -> Half-Open -> Closed).
     */
    void update() override;
};

#endif // PACMAN_PACMAN_VIEW_H