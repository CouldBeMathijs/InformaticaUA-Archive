/**
 * @file IEntityView.h
 * @brief Base class for all visual entity representations with animation support.
 */

#ifndef PACMAN_ENTITY_VIEW_H
#define PACMAN_ENTITY_VIEW_H

#include "../../logic/entityType/IEntityModel.h"
#include "../../logic/patterns/IObserver.h"
#include "../AssetManager.h"
#include "LogicConstants.h"
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * @class IEntityView
 * @brief Abstract base class that connects a logical entity to its visual sprite.
 * * This class implements the IObserver interface to react to changes in the model.
 * It provides built-in support for time-based sprite animations and tracks
 * which logical entity it is responsible for drawing.
 */
class IEntityView : public IObserver {
    const double m_timePerFrame = LogicConstants::ANIMATION_SPEED; /**< Duration of each animation frame in seconds. */
    double m_timeAccumulator = 0.0;                /**< Accumulated time to track when to switch frames. */
    std::shared_ptr<IEntityModel> m_coupledEntity; /**< The logic model being observed. */

protected:
    Assets::TextureLocation m_currentSprite; /**< The current texture/sprite ID from the AssetManager. */
    bool m_animationCycleMovingUp = true;    /**< Direction of animation (e.g., 0->1->2 then 2->1->0). */

    /**
     * @brief Protected constructor for derived view classes.
     * @param texture The initial texture location.
     * @param model The logical entity model to observe.
     */
    explicit IEntityView(Assets::TextureLocation texture, std::shared_ptr<IEntityModel> model);

    unsigned int m_amountOfTextures = 1;     /**< Total frames available for this entity's animation. */
    unsigned int m_currentTextureOffset = 0; /**< The specific frame index currently being displayed. */

public:
    /**
     * @brief Synchronizes the view's position with the model and handles animation timing.
     * * Implementation usually checks the Stopwatch for delta time to update the accumulator.
     */
    void update() override;

    /**
     * @brief Advances the animation frame based on m_amountOfTextures.
     * * Handles ping-pong (up and down) or looping animation logic.
     */
    void animate();

    /** @return The current frame index for texture selection. */
    [[nodiscard]] unsigned int getCurrentTextureOffset() const;

    /** @return The logical model associated with this view. */
    std::shared_ptr<IEntityModel> getCoupledEntity();
};

#endif // PACMAN_ENTITY_VIEW_H