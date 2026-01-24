#ifndef PACMAN_I_DIRECTIONAL_ENTITY_MODEL_H
#define PACMAN_I_DIRECTIONAL_ENTITY_MODEL_H
#include "IEntityModel.h"

class IDirectionalEntityModel : public IEntityModel {
protected:
    Position m_spawnPoint = getHitBox().topLeft;
    double m_speed;

public:
    IDirectionalEntityModel(const Rectangle& hitBox, Direction::Cardinal cardinal, double speed);

    /**
     * @return The current speed without Stopwatch multiplier
     */
    [[nodiscard]] double getSpeed() const;

    /**
     * @brief Move back to the position on which the entity was created
     */
    virtual void goToSpawn();

    /**
     * @brief Move one unit in the current Direction (without collision check)
     */
    virtual void move();

    /**
     * @param d The direction to set
     */
    void setDirection(Direction::Cardinal d);

    /**
     * @param hb The hitbox the entity should take in on the map
     */
    void setHitBox(const Rectangle& hb);
};

#endif
