/**
 * @file GhostView.h
 * @brief Visual representations for the different ghost entities.
 */

#ifndef PACMAN_GHOST_VIEW_H
#define PACMAN_GHOST_VIEW_H

#include "IDirectionalEntityView.h"

/**
 * @class IGhostView
 * @brief Base view class for all ghosts, managing shared state visuals.
 * * This class extends the directional view logic to include special
 * visual states like "Frightened" (blue/white flashing) and
 * "Eaten" (eyes only).
 */
class IGhostView : public IDirectionalEntityView {
    /** @brief The original texture location for the ghost's specific color. */
    Assets::TextureLocation m_baseSprite;

protected:
    /**
     * @brief Constructs a ghost view with a specific colored texture.
     * @param texture The unique texture for the specific ghost color.
     * @param model The logical ghost model being observed.
     */
    IGhostView(Assets::TextureLocation texture, std::shared_ptr<IEntityModel> model);

public:
    /**
     * @brief Updates the ghost's appearance based on game mode.
     * * Checks the model's GhostMode. If Frightened, it swaps the
     * texture to the generic "Blue Ghost" sprite; otherwise, it
     * reverts to m_baseSprite and updates directional orientation.
     */
    void update() override;
};

// --- Specialized Ghost Views ---

/** @brief View for Blinky (Red). */
class RedGhostView final : public IGhostView {
public:
    explicit RedGhostView(std::shared_ptr<IEntityModel> model);
};

/** @brief View for Pinky (Pink). */
class PinkGhostView final : public IGhostView {
public:
    explicit PinkGhostView(std::shared_ptr<IEntityModel> model);
};

/** @brief View for Inky (Blue). */
class BlueGhostView final : public IGhostView {
public:
    explicit BlueGhostView(std::shared_ptr<IEntityModel> model);
};

/** @brief View for Clyde (Orange). */
class OrangeGhostView final : public IGhostView {
public:
    explicit OrangeGhostView(std::shared_ptr<IEntityModel> model);
};

#endif // PACMAN_GHOST_VIEW_H