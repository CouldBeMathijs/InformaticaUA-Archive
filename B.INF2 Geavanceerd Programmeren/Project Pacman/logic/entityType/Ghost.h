#ifndef PACMAN_GHOST_H
#define PACMAN_GHOST_H
#include "IDirectionalEntityModel.h"
#include "LogicConstants.h"
#include "ScoreKeeper.h"

#include <iostream>
#include <stack>

/**
 * @brief Different movement modes Ghosts can be in
 */
enum class GhostMode { CHASING, PANICKING, WAITING, DEAD };

/**
 * @brief GhostMode to string
 * @param e
 * @return A string based representation of e
 */
const char* to_string(GhostMode e);

/**
 * @brief Different Algorithms Ghosts can use
 */
enum class ChasingAlgorithm { DIRECTIONAL, IN_FRONT_MANHATTAN, ON_TOP_MANHATTAN };

/**
 * @brief ChasingAlgorithm to string
 * @param e
 * @return A string based representation of e
 */
const char* to_string(ChasingAlgorithm e);

/**
 * @brief Struct to keep a GhostMode and timer together
 */
struct GhostState {
    GhostMode mode;
    double timer;
};

/**
 * @brief A stack of GhostStates
 */
class GhostModeStack final : std::stack<GhostState> {
public:
    using std::stack<GhostState>::empty;
    using std::stack<GhostState>::pop;
    using std::stack<GhostState>::push;
    using std::stack<GhostState>::size;
    using std::stack<GhostState>::top;
};

/**
 * @brief Virtual class to base on which to base specific Ghost types
 */
class IGhost : public IDirectionalEntityModel {
protected:
    ChasingAlgorithm m_algorithm;
    Direction::Cardinal m_wantedDirection = Direction::Cardinal::EAST;
    GhostModeStack m_stateStack;
    bool m_canMoveThroughSpawnDoor = true;
    double m_amount_of_seconds_until_able_to_turn = 0;
    double m_speed = LogicConstants::BASE_SPEED * (0.75 + 0.05 * ScoreKeeper::getInstance().getLevel());

    IGhost(const Rectangle& pos, GhostMode start_mode, double amountOfSecondsLeftInCurrentMode,
           ChasingAlgorithm algorithm);

public:
    /**
     * @return The ghosts current algorithm
     */
    [[nodiscard]] ChasingAlgorithm getAlgorithm() const;

    /**
     * @return The direction the ghost wants to move in
     */
    [[nodiscard]] Direction::Cardinal getWantedDirection() const;

    /**
     * @return Returns the top mode on the stateGhost stack
     */
    [[nodiscard]] GhostMode getMode() const;

    /**
     * @return True if the last turn was more than 0.1 seconds ago
     */
    [[nodiscard]] bool allowedToTurn() const;

    /**
     * @return True if the ghost can still move through the spawn door
     */
    [[nodiscard]] bool canMoveThroughSpawnDoor() const;

    /**
     * @return The amount of time left in the currrent mode, 0.0 means infinite
     */
    [[nodiscard]] double getTimeInCurrentMode() const;

    /**
     * @param touchingEntities All entities which the caller says touch the ghost
     * @return Whether a movement of this ghost must be blocked
     */
    bool isBlocked(const std::vector<std::shared_ptr<IEntityModel>>& touchingEntities) override;

    /**
     * @brief Calls the correct double dispatch
     * @param visitor Singularly dispatched visitor object
     */
    void accept(IEntityVisitor& visitor) override;

    /**
     * @brief Performs the routine of moving the ghost back to spawn
     */
    void die();

    /**
     * @brief Debug function: print this ghosts info to std::cout
     */
    void displayInfo() const;

    /**
     * @brief Go to spawn and reset flag for being allowed to move through spawn door;
     */
    void goToSpawn() override;

    /**
     * @brief Prevents movement through spawn door after function call
     */
    void hasExitedSpawn();

    /**
     * @brief Reset the timer used by allowedToTurn()
     */
    void hasTurned();

    /**
     * @brief Move one unit of movement in the current direction (no collision check)
     */
    void move() override;

    /**
     * @brief Add a ghostMode to the stack, if panic starts panic timer
     */
    void setMode(GhostMode);

    /**
     * @param d direction the ghost wants to move in
     */
    void setWantedDirection(Direction::Cardinal d);

    /**
     * @brief Update timers and call observer notify
     */
    void update(Direction::Cardinal) override;
};

/**
 * @brief Ghost following the IN_FRONT_MANHATTAN algorithm, waits 5 seconds to get moving
 */
class BlueGhost final : public IGhost {
public:
    explicit BlueGhost(const Rectangle& pos);
};

/**
 * @brief Ghost following the DIRECTIONAL algorithm
 */
class PinkGhost final : public IGhost {
public:
    explicit PinkGhost(const Rectangle& pos);
};

/**
 * @brief Ghost following the IN_FRONT_MANHATTAN algorithm
 */
class OrangeGhost final : public IGhost {
public:
    explicit OrangeGhost(const Rectangle& pos);
};

/**
 * @brief Ghost following the ON_TOP_MANHATTAN algorithm, waits 10 seconds to get moving
 */
class RedGhost final : public IGhost {
public:
    explicit RedGhost(const Rectangle& pos);
};

#endif // PACMAN_GHOST_H
