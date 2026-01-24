#ifndef PACMAN_PACMAN_H
#define PACMAN_PACMAN_H
#include "IDirectionalEntityModel.h"

/**
 * @brief The player character
 */
class Pacman final : public IDirectionalEntityModel {
    bool m_ghostTouch = false;
    double m_deathTimer = 0;

public:
    explicit Pacman(const Rectangle& pos);
    /**
     * @brief Getter for ghostTouch boolean
     * @return True if pacman has touched a Ghost, else false
     */
    [[nodiscard]] bool hasTouchedGhost() const;

    void goToSpawn() override;

    /**
     * @brief Calls the correct double dispatch
     * @param visitor Singularly dispatched visitor object
     */
    void accept(IEntityVisitor& visitor) override;

    /**
     * @brief Makes it so hasTouchedGhost() returns true until resetGhostTouch() is called
     */
    void ghostTouches();

    void update(Direction::Cardinal) override;

    /**
     * @brief Makes it so hasTouchedGhost() returns false until ghostTouches() is called
     */
    void resetGhostTouch();

    [[nodiscard]] double getDeathTimer() const;

    void die();
};

#endif // PACMAN_PACMAN_H
