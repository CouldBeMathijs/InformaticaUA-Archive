#ifndef PACMAN_VISITOR_H
#define PACMAN_VISITOR_H

#include "../entityType/Collectable.h"
#include "../entityType/Ghost.h"
#include "../entityType/Pacman.h"

// --- Forward Declarations ---
class Fruit;
class IEntityModel;
class IGhost;
class Pacman;
class SpawnWall;
class Wall;

/**
 * @brief Used to pass back information about the collision.
 */
enum class CollisionResult { NONE, MOVE_BLOCKED, SPAWN_WALL_BLOCK, GHOST_TOUCH, COIN_PICKED_UP, FRUIT_PICKED_UP };

/**
 * @brief The core Visitor Interface for all dispatch layers.
 */
class IEntityVisitor {
public:
    virtual ~IEntityVisitor() = default;

    // A visit method for every concrete element type
    virtual void visit(Coin& coin) = 0;
    virtual void visit(Fruit& fruit) = 0;
    virtual void visit(IGhost& ghost) = 0;
    virtual void visit(Pacman& pacman) = 0;
    virtual void visit(SpawnWall& spawn_wall) = 0;
    virtual void visit(Wall& wall) = 0;
};

/**
 * @brief Handles logic when PACMAN is the initiator.
 */
class PacmanCollisionVisitor final : public IEntityVisitor {
    CollisionResult& m_result;

public:
    explicit PacmanCollisionVisitor(CollisionResult& result) : m_result(result) {}

    void visit(Pacman& target) override {} // Pacman vs Pacman ignored

    void visit(IGhost& target) override;

    void visit(Wall& target) override;

    void visit(Coin& target) override;

    void visit(Fruit& target) override;

    void visit(SpawnWall& target) override;
};

/**
 * @brief Handles logic when GHOST is the initiator.
 */
class GhostCollisionVisitor final : public IEntityVisitor {
    CollisionResult& m_result;

public:
    explicit GhostCollisionVisitor(CollisionResult& result) : m_result(result) {}

    void visit(IGhost& target) override;

    void visit(Pacman& target) override;

    void visit(Wall& target) override;

    void visit(SpawnWall& target) override;

    // Ghosts ignore items
    void visit(Coin& target) override {}

    void visit(Fruit& target) override {}
};

/**
 * @brief Handles picking up collectables
 */
class CollectableVisitor final : public IEntityVisitor {
public:
    void visit(Pacman& pacman) override {}

    void visit(IGhost& ghost) override {}

    void visit(Wall& wall) override {}

    void visit(SpawnWall& spawn_wall) override {}

    void visit(Coin& coin) override;

    void visit(Fruit& fruit) override;
};

/**
 * @brief A temporary Visitor used to resolve the Initiator's type.
 * * It holds a reference to the TARGET.
 * We pass this visitor to the INITIATOR.
 * When the INITIATOR calls visit(InitiatorType), we finally know both types:
 * 1. The InitiatorType (via the function called)
 * 2. The TargetType (via the member variable)
 */
template <typename TargetType>
class CollisionResolver final : public IEntityVisitor {
    TargetType& m_target;
    CollisionResult& m_result;

public:
    CollisionResolver(TargetType& target, CollisionResult& res) : m_target(target), m_result(res) {}

    // 1. If the Initiator identifies itself as Pacman:
    void visit(Pacman& initiator) override;

    // 2. If the Initiator identifies itself as Ghost:
    void visit(IGhost& initiator) override {
        // Create the logic handler for Ghost...
        GhostCollisionVisitor logic(m_result);
        // ...and apply it to the specific target we are holding.
        logic.visit(m_target);
    }

    // The Initiator in this game is usually dynamic (Pacman/Ghost),
    // so it is unlikely a Wall will be the Initiator, but we must implement the interface.
    void visit(Wall& wall) override {}

    void visit(Coin& coin) override {}

    void visit(Fruit& fruit) override {}

    void visit(SpawnWall& spawn_wall) override {}
};

template <typename TargetType>
void CollisionResolver<TargetType>::visit(Pacman& initiator) {
    // Create the logic handler for Pacman...
    PacmanCollisionVisitor logic(m_result);
    // ...and apply it to the specific target we are holding.
    logic.visit(m_target);
}

/**
 * @brief Handles the first dispatch
 */
class CollisionHandler final : public IEntityVisitor {
private:
    IEntityModel& m_initiator;
    CollisionResult m_result;

public:
    explicit CollisionHandler(IEntityModel& initiator) : m_initiator(initiator) {}

    [[nodiscard]] const CollisionResult& getResult() const { return m_result; }

    // --- The bridging methods ---
    // Instead of casting, we create a specific Resolver for the Target,
    // and ask the Initiator to accept it.

    void visit(Pacman& target) override;

    void visit(IGhost& target) override;

    void visit(Wall& target) override;

    void visit(Coin& target) override;

    void visit(Fruit& target) override;

    void visit(SpawnWall& target) override;
};

#endif // PACMAN_VISITOR_H