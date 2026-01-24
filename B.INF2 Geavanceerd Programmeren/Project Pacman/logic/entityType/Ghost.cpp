#include "Ghost.h"

#include "../patterns/Visitor.h"
#include "Random.h"
#include "ScoreKeeper.h"
#include "Stopwatch.h"

const char* to_string(const GhostMode e) {
    switch (e) {
    case GhostMode::CHASING:
        return "CHASING";
    case GhostMode::PANICKING:
        return "PANICKING";
    case GhostMode::WAITING:
        return "WAITING";
    case GhostMode::DEAD:
        return "DEAD";
    default:
        return "unknown";
    }
}

const char* to_string(const ChasingAlgorithm e) {
    switch (e) {
    case ChasingAlgorithm::DIRECTIONAL:
        return "DIRECTIONAL";
    case ChasingAlgorithm::IN_FRONT_MANHATTAN:
        return "IN_FRONT_MANHATTAN";
    case ChasingAlgorithm::ON_TOP_MANHATTAN:
        return "ON_TOP_MANHATTAN";
    default:
        return "unknown";
    }
}

IGhost::IGhost(const Rectangle& pos, const GhostMode start_mode, const double amountOfSecondsLeftInCurrentMode,
               const ChasingAlgorithm algorithm)
    : IDirectionalEntityModel(pos, Direction::Cardinal::EAST, LogicConstants::BASE_SPEED * 0.8),
      m_algorithm(algorithm) {
    m_stateStack.push({GhostMode::CHASING, 0.0});
    if (start_mode != GhostMode::CHASING) {
        m_stateStack.push({start_mode, amountOfSecondsLeftInCurrentMode});
    }
}

void IGhost::accept(IEntityVisitor& visitor) { visitor.visit(*this); }

void IGhost::die() {
    if (getMode() == GhostMode::PANICKING) {
        m_stateStack.pop();
    }
    goToSpawn();
}

GhostMode IGhost::getMode() const { return m_stateStack.top().mode; }

double IGhost::getTimeInCurrentMode() const { return m_stateStack.top().timer; }

void IGhost::setMode(const GhostMode m) {
    if (m == GhostMode::PANICKING) {
        // If already panicking, just reset the top timer
        if (!m_stateStack.empty() && m_stateStack.top().mode == GhostMode::PANICKING) {
            m_stateStack.top().timer = std::max(10.0 - ScoreKeeper::getInstance().getLevel(), 2.0);
        } else {
            // Push Panic onto the stack with 10s
            m_stateStack.push({GhostMode::PANICKING, 10.0});

            // Only flip if not in the house
            if (m_stateStack.size() > 1 && m_stateStack.top().mode != GhostMode::WAITING) {
                setDirection(getOpposite(getDirection()));
                setWantedDirection(getOpposite(getWantedDirection()));
            }
        }
    } else {
        // For hard state changes (like DEAD or spawning), clear and set base state
        while (!m_stateStack.empty())
            m_stateStack.pop();
        m_stateStack.push({m, 0.0}); // Timer 0 for infinite states like CHASING
    }
}

ChasingAlgorithm IGhost::getAlgorithm() const { return m_algorithm; }

Direction::Cardinal IGhost::getWantedDirection() const { return m_wantedDirection; }

void IGhost::setWantedDirection(const Direction::Cardinal d) { m_wantedDirection = d; }

bool IGhost::canMoveThroughSpawnDoor() const {
    if (m_stateStack.empty() || getMode() == GhostMode::WAITING) {
        return false;
    }

    return m_canMoveThroughSpawnDoor;
}

bool IGhost::allowedToTurn() const { return m_amount_of_seconds_until_able_to_turn <= 0; }

void IGhost::goToSpawn() {
    m_direction = Direction::Cardinal::EAST;
    m_canMoveThroughSpawnDoor = true;
    m_stateStack.push({GhostMode::WAITING, Random::getInstance().generateDouble(0.5, 5)});
    IDirectionalEntityModel::goToSpawn();
}

void IGhost::hasTurned() { m_amount_of_seconds_until_able_to_turn = 0.1; }

void IGhost::update(const Direction::Cardinal direction) {
    const double dt = Stopwatch::getInstance().getDeltaTime();

    if (!m_stateStack.empty()) {

        // If the current state has a timer (Panic or Waiting)
        if (auto& [mode, timer] = m_stateStack.top(); timer > 0) {
            timer -= dt;

            // If timer expires, pop the state!
            if (timer <= 0) {
                m_stateStack.pop();
            }
        }
    }

    // Standard logic
    if (m_amount_of_seconds_until_able_to_turn > 0) {
        m_amount_of_seconds_until_able_to_turn -= dt;
    }
    IEntityModel::update(direction);
}

bool IGhost::isBlocked(const std::vector<std::shared_ptr<IEntityModel>>& touchingEntities) {
    const auto entity = this;
    for (const auto& target_ptr : touchingEntities) {

        if (target_ptr.get() == entity) {
            continue;
        }

        CollisionHandler entityInitiates(*entity);
        target_ptr->accept(entityInitiates);
        if (entityInitiates.getResult() == CollisionResult::MOVE_BLOCKED ||
            (entityInitiates.getResult() == CollisionResult::SPAWN_WALL_BLOCK && !m_canMoveThroughSpawnDoor)) {
            return true;
        }

        CollisionHandler targetInitiates(*target_ptr);
        entity->accept(targetInitiates);
        if (targetInitiates.getResult() == CollisionResult::MOVE_BLOCKED ||
            (entityInitiates.getResult() == CollisionResult::SPAWN_WALL_BLOCK && !m_canMoveThroughSpawnDoor)) {
            return true;
        }
    }
    return false;
}

void IGhost::displayInfo() const {
    const auto [mode, timer] = m_stateStack.top();
    std::cout << "Algorithm: " << to_string(m_algorithm) << "\n\tPosition: " << m_hitBox.getCenter()
              << "\n\tCanMoveThroughSpawnDoor: " << m_canMoveThroughSpawnDoor << "\n\tCurrentMode: " << to_string(mode)
              << "\n\tTimeLeftInCurrentMode: " << timer << std::endl;
}

void IGhost::move() {
    if (getMode() == GhostMode::PANICKING) {
        m_hitBox.moveBy(getValue(m_direction) * Stopwatch::getInstance().getDeltaTime() * m_speed * 0.6);
    } else {
        IDirectionalEntityModel::move();
    }
}

void IGhost::hasExitedSpawn() { m_canMoveThroughSpawnDoor = false; }

BlueGhost::BlueGhost(const Rectangle& pos) : IGhost(pos, GhostMode::WAITING, 5, ChasingAlgorithm::IN_FRONT_MANHATTAN) {}

PinkGhost::PinkGhost(const Rectangle& pos) : IGhost(pos, GhostMode::CHASING, 0, ChasingAlgorithm::DIRECTIONAL) {}

OrangeGhost::OrangeGhost(const Rectangle& pos)
    : IGhost(pos, GhostMode::WAITING, 0.5, ChasingAlgorithm::IN_FRONT_MANHATTAN) {}

RedGhost::RedGhost(const Rectangle& pos) : IGhost(pos, GhostMode::WAITING, 10, ChasingAlgorithm::ON_TOP_MANHATTAN) {}
