#ifndef PACMAN_COLLECTABLE_H
#define PACMAN_COLLECTABLE_H
#include "IEntityModel.h"

/**
 * @brief Virtual class for all collectables
 */
class Collectable : public IEntityModel {
protected:
    bool m_isPickedUp = false;
    int m_value = 0;
    explicit Collectable(const Rectangle& pos, const Direction::Cardinal d);

public:
    /**
     * @brief Tell the entity model it is picked up, stop updating Observers after one last call out and update the
     * Score
     */
    void bePickedUp();

    /**
     * @brief Update the observers if not yet picked up
     */
    void update(Direction::Cardinal) override;

    /**
     * @brief Set the value to be passed through the Score after being picked up
     */
    void setValue(int);
};

/**
 * @brief Can be picked up by Pacman and increases the score when picked up
 */
class Coin final : public Collectable {
public:
    /**
     *
     * @param pos Position of the Entity at World start
     */
    explicit Coin(const Rectangle& pos);

    /**
     * @brief Calls the correct double dispatch
     * @param visitor visitor
     */
    void accept(IEntityVisitor& visitor) override;
};

/**
 * @brief Can be picked up by Pacman, increases the score and causes ghost panic when picked up
 */
class Fruit final : public Collectable {
public:
    /**
     *
     * @param pos Position of Entity at World start
     */
    explicit Fruit(const Rectangle& pos);

    /**
     * @brief Calls the correct double dispatch
     * @param visitor visitor
     */
    void accept(IEntityVisitor& visitor) override;
};

#endif // PACMAN_COLLECTABLE_H
