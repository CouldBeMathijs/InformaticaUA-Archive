#ifndef PACMAN_ENTITY_MODEL_H
#define PACMAN_ENTITY_MODEL_H
#include "../Position.h"
#include "ISubject.h"

class IEntityModel : public ISubject {
protected:
    IEntityModel(Rectangle hitBox, Direction::Cardinal);
    Direction::Cardinal m_direction;
    Rectangle m_hitBox;
    bool m_isMarkedForDeletion = false;

public:
    /**
     * @return The current direction
     */
    [[nodiscard]] Direction::Cardinal getDirection() const;

    /**
     * @return The current hit box
     */
    [[nodiscard]] Rectangle getHitBox() const;

    /**
     * Checks whether the entity overlaps with the bound box
     * @param boundBox Box to check overlap with
     * @return Whether there is overlap
     */
    [[nodiscard]] bool isInBounds(const Rectangle& boundBox) const;

    /**
     * @return The m_isMarkedForDeletion flag
     */
    [[nodiscard]] bool isMarkedForDeletion() const;

    /**
     * @param touchingEntities All entities which the caller says touch the ghost
     * @return Whether a movement of this ghost must be blocked
     */
    [[nodiscard]] virtual bool isBlocked(const std::vector<std::shared_ptr<IEntityModel>>& touchingEntities);

    /**
     * @brief Static method to calculate the hitBox when not wanting to edit the member variables
     * @param current_hb The hit box to start with
     * @param d The direction to move in
     * @param distance The distance to move in
     * @return The new hitbox after the move
     */
    static Rectangle calculateFutureHitBox(const Rectangle& current_hb, Direction::Cardinal d, double distance);

    /**
     * @brief Set the m_isMarkedForDeletion flag to true
     */
    void markForDeletion();

    /**
     * @brief Snaps to the grid when extremely close to time center
     */
    void snapToGrid();
};

#endif
