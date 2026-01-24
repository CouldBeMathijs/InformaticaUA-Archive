#include "Visitor.h"

void PacmanCollisionVisitor::visit(IGhost& target) {
    // Pacman vs Ghost: Game Over or Eat Ghost
    m_result = CollisionResult::GHOST_TOUCH;
}

void PacmanCollisionVisitor::visit(Wall& target) {
    // Pacman vs Wall: Blocked
    m_result = CollisionResult::MOVE_BLOCKED;
}

void PacmanCollisionVisitor::visit(Coin& target) {
    // Pacman vs Coin: Pickup interaction noted
    m_result = CollisionResult::COIN_PICKED_UP;
}

void PacmanCollisionVisitor::visit(Fruit& target) {
    // Pacman vs Fruit: Pickup interaction noted
    m_result = CollisionResult::FRUIT_PICKED_UP;
}

void PacmanCollisionVisitor::visit(SpawnWall& target) {
    // Pacman vs SpawnWall
    m_result = CollisionResult::MOVE_BLOCKED;
}

void GhostCollisionVisitor::visit(IGhost& target) {
    // Ghost vs Ghost logic
    // m_result = CollisionResult::MOVE_BLOCKED;
}

void GhostCollisionVisitor::visit(Pacman& target) {
    // Ghost vs Pacman
    m_result = CollisionResult::GHOST_TOUCH;
}

void GhostCollisionVisitor::visit(Wall& target) {
    // Ghost vs Wall: Blocked
    m_result = CollisionResult::MOVE_BLOCKED;
}

void GhostCollisionVisitor::visit(SpawnWall& target) {
    // Ghost vs SpawnWall: should block when not moving away from spawn
    m_result = CollisionResult::SPAWN_WALL_BLOCK;
}

void CollectableVisitor::visit(Coin& coin) { coin.bePickedUp(); }

void CollectableVisitor::visit(Fruit& fruit) { fruit.bePickedUp(); }

void CollisionHandler::visit(Pacman& target) {
    CollisionResolver<Pacman> resolver(target, m_result);
    m_initiator.accept(resolver);
}

void CollisionHandler::visit(IGhost& target) {
    CollisionResolver<IGhost> resolver(target, m_result);
    m_initiator.accept(resolver);
}

void CollisionHandler::visit(Wall& target) {
    // Logic: Initiator.accept(ResolverThatHoldsWall)
    CollisionResolver<Wall> resolver(target, m_result);
    m_initiator.accept(resolver);
}

void CollisionHandler::visit(Coin& target) {
    CollisionResolver<Coin> resolver(target, m_result);
    m_initiator.accept(resolver);
}

void CollisionHandler::visit(Fruit& target) {
    CollisionResolver<Fruit> resolver(target, m_result);
    m_initiator.accept(resolver);
}

void CollisionHandler::visit(SpawnWall& target) {
    CollisionResolver<SpawnWall> resolver(target, m_result);
    m_initiator.accept(resolver);
}
