/**
 * @file World.h
 * @brief Management of game entities, collision detection, and world-state transitions.
 */

#ifndef PACMAN_WORLD_H
#define PACMAN_WORLD_H

#include "entityType/Ghost.h"
#include "entityType/ISubject.h"
#include "entityType/Pacman.h"

#include <memory>
#include <set>
#include <vector>

/**
 * @enum WorldState
 * @brief High-level states representing the current phase of the game session.
 */
enum class WorldState {
    RUNNING,  /**< Normal gameplay is active. */
    VICTORY,  /**< All collectables have been gathered. */
    GAME_OVER /**< Pac-Man has lost all lives. */
};

/**
 * @class World
 * @brief The container and controller for all active game entities.
 * * This class facilitates interaction between Pac-Man, the Ghosts, and the
 * environment (walls, pellets). it provides spatial logic for AI navigation
 * and collision responses.
 */
class World {
    WorldState m_worldState = WorldState::RUNNING;
    std::shared_ptr<Pacman> m_pacman;
    std::vector<std::shared_ptr<IEntityModel>> m_nonMovingEntities; /**< Walls, Pellets, Fruit. */
    std::vector<std::shared_ptr<IGhost>> m_ghosts;

protected:
    /** @brief Checks if Pac-Man's hitbox intersects with items and triggers scoring. */
    void handleCollectables(const Rectangle& current_hb);

    /** @brief Moves Pac-Man in the specified direction and checks for wall collisions. */
    void updatePacman(Direction::Cardinal d);

    /** @brief Triggers the "Frightened" state for all ghosts. */
    void startPanic() const;

public:
    World() = default;

    /**
     * @brief AI Pathfinding logic based on tile distance.
     * * Chooses the next direction for a ghost by evaluating which available neighbor
     * tile is geographically closest to (or furthest from) the target position.
     * @param wantedManhattan The target coordinate the ghost wants to reach.
     * @param ghost The ghost making the decision.
     * @param maximizeDistance If true, the ghost moves away from the target (used for Frightened mode).
     * @return The optimal Cardinal direction to move next.
     */
    Direction::Cardinal manhattanDecision(const Position& wantedManhattan, const std::shared_ptr<IGhost>& ghost,
                                          bool maximizeDistance);

    [[nodiscard]] WorldState getState() const;
    [[nodiscard]] const std::shared_ptr<Pacman>& getPacman() const;
    [[nodiscard]] const std::vector<std::shared_ptr<IEntityModel>>& getNonMovingEntities() const;
    [[nodiscard]] const std::vector<std::shared_ptr<IGhost>>& getGhosts() const;

    /**
     * @brief Identifies valid exit directions from a ghost's current tile.
     * * Excludes directions that lead into walls or, typically, the opposite of the ghost's current heading.
     * @param ghost The entity to check.
     * @return A set of available Cardinal directions.
     */
    std::set<Direction::Cardinal> possibleDirections(const std::shared_ptr<IGhost>& ghost);

    /**
     * @brief Spatial query to find entities within a specific area.
     * @param boundBox The rectangular area to check.
     * @return A list of shared pointers to entities overlapping the box.
     */
    std::vector<std::shared_ptr<IEntityModel>> getEntitiesInBounds(const Rectangle& boundBox);

    // --- Entity Management ---
    void addGhost(std::shared_ptr<IGhost> ghost);
    void addNonMovingEntity(std::shared_ptr<IEntityModel>);
    void setPacman(const std::shared_ptr<Pacman>&);

    /**
     * @brief Advances the world simulation by one tick.
     * @param d The current input direction for Pac-Man.
     */
    void update(Direction::Cardinal d);

    /** @brief Updates AI logic and movement for all active ghosts. */
    void updateGhosts(Direction::Cardinal d);
};

#endif // PACMAN_WORLD_H