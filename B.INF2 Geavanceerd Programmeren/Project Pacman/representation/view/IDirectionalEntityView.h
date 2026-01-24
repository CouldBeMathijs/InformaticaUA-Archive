/**
 * @file IDirectionalEntityView.h
 * @brief Base class for entities whose appearance depends on their movement direction.
 */

#ifndef PACMAN_I_DIRECTIONAL_ENTITY_VIEW_H
#define PACMAN_I_DIRECTIONAL_ENTITY_VIEW_H

#include "IEntityView.h"

/**
 * @class IDirectionalEntityView
 * @brief Extends IEntityView to handle orientation-based sprite selection.
 * * This class manages entities that have different sprite rows or offsets
 * for each cardinal direction. It automatically calculates the correct
 * texture coordinates based on the coupled entity's current Direction.
 */
class IDirectionalEntityView : public IEntityView {
protected:
    /** * @brief The baseline vertical coordinate in the spritesheet.
     * Used as a reference point to calculate offsets for different directions.
     */
    int m_topBase = m_currentSprite.top;

    /**
     * @brief Constructs a directional view.
     * @param m The initial texture location.
     * @param n The logical entity model to observe.
     * @param amountOfTextures The number of animation frames available per direction.
     */
    IDirectionalEntityView(Assets::TextureLocation m, std::shared_ptr<IEntityModel> n, int amountOfTextures);

public:
    /**
     * @brief Updates the view's position and adjusts texture rects based on direction.
     * * Overrides the base update to ensure the sprite face matches the
     * entity's movement direction before calling the animation logic.
     */
    void update() override;
};

#endif // PACMAN_I_DIRECTIONAL_ENTITY_VIEW_H