#include "World.h"

#include "LogicConstants.h"
#include "Random.h"
#include "ScoreKeeper.h"
#include "Stopwatch.h"
#include "entityType/Pacman.h"
#include "patterns/IEntityFactory.h"
#include "patterns/Visitor.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <ranges>
#include <vector>

void World::addNonMovingEntity(std::shared_ptr<IEntityModel> e) { m_nonMovingEntities.emplace_back(std::move(e)); }

void World::addGhost(std::shared_ptr<IGhost> ghost) { m_ghosts.emplace_back(std::move(ghost)); }

const std::vector<std::shared_ptr<IEntityModel>>& World::getNonMovingEntities() const { return m_nonMovingEntities; }

const std::vector<std::shared_ptr<IGhost>>& World::getGhosts() const { return m_ghosts; }

std::vector<std::shared_ptr<IEntityModel>> World::getEntitiesInBounds(const Rectangle& boundBox) {
    std::vector<std::shared_ptr<IEntityModel>> results;
    // Non-moving entities within bounding box
    std::ranges::copy_if(m_nonMovingEntities,
                         std::back_inserter(results), // Efficiently adds elements to the results vector
                         [&boundBox](const std::shared_ptr<IEntityModel>& entity) {
                             // Check if the entity (which is a shared_ptr) is not null and is in bounds.
                             return entity && entity->isInBounds(boundBox);
                         });
    // Ghosts within bounding box
    std::ranges::copy_if(m_ghosts,
                         std::back_inserter(results), // Efficiently adds elements to the results vector
                         [&boundBox](const std::shared_ptr<IEntityModel>& ghost) {
                             // Check if the entity (which is a shared_ptr) is not null and is in bounds.
                             return ghost && ghost->isInBounds(boundBox);
                         });
    // Pacman if within bounding box
    if (m_pacman->isInBounds(boundBox))
        results.push_back(m_pacman);

    return results;
}

void World::setPacman(const std::shared_ptr<Pacman>& p) { m_pacman = p; }

const std::shared_ptr<Pacman>& World::getPacman() const { return m_pacman; }

/**
 * @brief Updates the state of the game world, handling Pacman's movement,
 * collision resolution, interactions (like picking up pellets), and
 * the updates of all other entities.
 *
 * @param d The direction input received from the user for Pacman's intended movement.
 */
void World::update(const Direction::Cardinal d) {
    auto& score = ScoreKeeper::getInstance();
    if (!m_pacman) {
        throw std::runtime_error("Pacman not defined");
    }
    if (score.collectablesLeft() == 0) {
        m_worldState = WorldState::VICTORY;
        return;
    }
    for (const auto& entity_ptr : m_nonMovingEntities) {
        entity_ptr->update(d);
    }
    ScoreKeeper::getInstance().update();

    updatePacman(d);
    updateGhosts(d);

    if (m_pacman->hasTouchedGhost()) {
        score.removeLife();
        if (score.getLives() == 0) {
            m_worldState = WorldState::GAME_OVER;
            return;
        }
        m_pacman->die();
        m_pacman->resetGhostTouch();
        for (const auto& ghost : m_ghosts) {
            ghost->goToSpawn();
        }
    }
    std::erase_if(m_nonMovingEntities, [](const auto& e) { return e->isMarkedForDeletion(); });
}

std::set<Direction::Cardinal> World::possibleDirections(const std::shared_ptr<IGhost>& ghost) {
    const double distance = ghost->getSpeed() * Stopwatch::getInstance().getDeltaTime();
    const double lookahead = distance * 10;
    const auto hitBox = ghost->getHitBox().scaledBy(0.9);

    // Try four primary directions
    constexpr std::array<Direction::Cardinal, 4> test = {Direction::Cardinal::EAST, Direction::Cardinal::WEST,
                                                         Direction::Cardinal::NORTH, Direction::Cardinal::SOUTH};

    std::set<Direction::Cardinal> out;

    // A direction is valid only if it's NOT blocked
    for (auto dir : test) {
        if (!ghost->isBlocked(getEntitiesInBounds(hitBox.movedBy(getValue(dir) * lookahead))))
            out.insert(dir);
    }

    if (!ghost->isBlocked(getEntitiesInBounds(hitBox.movedBy(getValue(ghost->getDirection()) * distance * 2)))) {
        out.insert(ghost->getDirection());
    }

    // If the ghost is directly facing a wall, keep side movement
    // (the above logic already allows this, but ensure reverse isn't forbidden)
    const Direction::Cardinal opposite = getOpposite(ghost->getDirection());

    // Normal rule: ghosts can't reverse
    // Exception: if blocked forward AND side paths exist, allow choosing sides
    if (out.size() > 1) {
        out.erase(opposite);
    }

    return out;
}

Direction::Cardinal World::manhattanDecision(const Position& wantedManhattan, const std::shared_ptr<IGhost>& ghost,
                                             const bool maximizeDistance) {
    const auto possible = possibleDirections(ghost);
    const double distance = ghost->getSpeed() * Stopwatch::getInstance().getDeltaTime();

    auto bestDir = Direction::Cardinal::EAST;
    // Initialize bestDistance with an extreme value
    double bestDistance = maximizeDistance ? -1.0 : std::numeric_limits<double>::max();

    for (const auto dir : possible) {
        // Predict the new position based on direction and speed
        auto hitbox = ghost->getHitBox().movedBy(getValue(dir) * distance);
        const auto manhattan = hitbox.getCenter().manhattan_distance_to(wantedManhattan);

        if (maximizeDistance) {
            // Looking for the largest distance
            if (manhattan > bestDistance) {
                bestDistance = manhattan;
                bestDir = dir;
            }
        } else {
            // Looking for the smallest distance
            if (manhattan < bestDistance) {
                bestDistance = manhattan;
                bestDir = dir;
            }
        }
    }

    return bestDir;
}

void World::updateGhosts(const Direction::Cardinal d) {
    for (const auto& ghost : m_ghosts) {
        const double moveDist = ghost->getSpeed() * Stopwatch::getInstance().getDeltaTime();
        ghost->snapToGrid();
        ghost->update(d);
        if (ghost->canMoveThroughSpawnDoor()) {
            if (const auto ghostX = ghost->getHitBox().getCenter().x; std::abs(ghostX) > moveDist) {
                if (ghostX < 0) {
                    ghost->setDirection(Direction::Cardinal::EAST);
                } else {
                    ghost->setDirection(Direction::Cardinal::WEST);
                }
            } else {
                ghost->setDirection(Direction::Cardinal::NORTH);
            }
            ghost->move();
            if (ghost->isBlocked(getEntitiesInBounds(ghost->getHitBox().scaledBy(0.99)))) {
                ghost->hasExitedSpawn();
            } else {
                continue;
            }
        }

        switch (ghost->getMode()) {
        case GhostMode::WAITING: {
            const double lookAheadDist = moveDist * 2;
            Rectangle lookAheadBox =
                IEntityModel::calculateFutureHitBox(ghost->getHitBox(), ghost->getWantedDirection(), lookAheadDist);

            // Check if the ghost is about to hit a wall in its current "wanted" direction
            if (ghost->isBlocked(getEntitiesInBounds(lookAheadBox.scaledBy(0.9)))) {
                // If blocked, flip the direction (Left <-> Right)
                if (ghost->getWantedDirection() == Direction::Cardinal::EAST) {
                    ghost->setWantedDirection(Direction::Cardinal::WEST);
                } else {
                    ghost->setWantedDirection(Direction::Cardinal::EAST);
                }
            }

            // Apply the direction and move
            ghost->setDirection(ghost->getWantedDirection());
            ghost->move();
        } break;
        case GhostMode::CHASING: {
            // Define the movement distance for this specific frame

            // Define the lookahead distance (10 "steps")
            // This ensures we don't move unless the long-range path is clear
            const double lookAheadDist = moveDist * 2;

            // 1. MOVEMENT CHECK:
            // We check the long distance (lookAheadDist). If that is clear, we perform the short move (moveDist).
            // This satisfies "look further ahead than you can move" and "only move if you can move 10 steps".
            Rectangle lookAheadBox =
                IEntityModel::calculateFutureHitBox(ghost->getHitBox(), ghost->getWantedDirection(), lookAheadDist);

            if (!ghost->isBlocked(getEntitiesInBounds(lookAheadBox.scaledBy(0.9)))) {
                // The long path is clear, so we can safely nudge the ghost forward
                ghost->setDirection(ghost->getWantedDirection());
                ghost->move();
            }
            if (ghost->allowedToTurn()) {
                // 2. DIRECTION DECISION LOGIC
                switch (ghost->getAlgorithm()) {
                case ChasingAlgorithm::ON_TOP_MANHATTAN:
                    ghost->setWantedDirection(manhattanDecision(m_pacman->getHitBox().getCenter(), ghost, false));
                    break;
                case ChasingAlgorithm::IN_FRONT_MANHATTAN:
                    ghost->setWantedDirection(manhattanDecision(
                        m_pacman->getHitBox().movedBy(Direction::getValue(m_pacman->getDirection())).getCenter(), ghost,
                        false));
                    break;
                case ChasingAlgorithm::DIRECTIONAL: {
                    if (auto possible = possibleDirections(ghost); !possible.empty()) {
                        if (possible.size() > 1) {
                            ghost->hasTurned();
                        }
                        ghost->setWantedDirection(Random::getInstance().getRandomElement(possible));
                    }
                }
                }
            }
        } break;
        case GhostMode::PANICKING: {
            // Define the movement distance for this specific frame

            // Define the lookahead distance (10 "steps")
            // This ensures we don't move unless the long-range path is clear
            const double lookAheadDist = moveDist * 2;

            // 1. MOVEMENT CHECK:
            // We check the long distance (lookAheadDist). If that is clear, we perform the short move (moveDist).
            // This satisfies "look further ahead than you can move" and "only move if you can move 10 steps".
            Rectangle lookAheadBox =
                IEntityModel::calculateFutureHitBox(ghost->getHitBox(), ghost->getWantedDirection(), lookAheadDist);

            if (!ghost->isBlocked(getEntitiesInBounds(lookAheadBox.scaledBy(0.9)))) {
                // The long path is clear, so we can safely nudge the ghost forward
                ghost->setDirection(ghost->getWantedDirection());
                ghost->move();
            }
            ghost->setWantedDirection(manhattanDecision(m_pacman->getHitBox().getCenter(), ghost, true));
        } break;
        case GhostMode::DEAD:
            ghost->goToSpawn();
            break;
        }
    }
}

void World::updatePacman(Direction::Cardinal d) {
    m_pacman->snapToGrid();
    if (m_pacman->getDeathTimer() > 0) {
        m_pacman->update(d);
        return;
    }
    constexpr double EPSILON = 0.01;
    const Rectangle current_hb = m_pacman->getHitBox();
    const double current_speed =
        std::round(60 * Stopwatch::getInstance().getDeltaTime()) * m_pacman->getSpeed() * 1.f / 60;

    if (const Direction::Cardinal current_direction = m_pacman->getDirection(); d != current_direction) {
        constexpr double lookahead_speed = 0.05; // Lookahead distance

        // 1. Check the intended direction normally
        const Rectangle future_hb_check_unscaled = IEntityModel::calculateFutureHitBox(current_hb, d, lookahead_speed);
        const Rectangle future_hb_check_scaled = future_hb_check_unscaled.scaledBy(1 - EPSILON);

        bool lookaheadBlocked = m_pacman->isBlocked(getEntitiesInBounds(future_hb_check_scaled));

        // 2. CORNER CUTTING LOGIC
        // If the direct path is blocked, check perpendicular offsets
        if (lookaheadBlocked) {
            constexpr double CORNER_TOLERANCE = LogicConstants::TILE_HEIGHT / 6;

            // Determine if we are moving vertically or horizontally
            const bool isVerticalChange = d == Direction::Cardinal::NORTH || d == Direction::Cardinal::SOUTH;

            // Check offsets: Negative (Left/Up) and Positive (Right/Down)

            for (double offsets[] = {-CORNER_TOLERANCE, CORNER_TOLERANCE}; const double offset : offsets) {
                // Create a temporary "snapped" current position
                Rectangle shifted_current_hb = current_hb;

                if (isVerticalChange) {
                    shifted_current_hb.moveBy(offset, 0); // Shift X for vertical movement
                } else {
                    shifted_current_hb.moveBy(0, offset); // Shift Y for horizontal movement
                }

                // Calculate lookahead from the SHIFTED position
                Rectangle shifted_future = IEntityModel::calculateFutureHitBox(shifted_current_hb, d, lookahead_speed);

                // If this shifted path is clear, we found a corner!
                if (Rectangle shifted_check = shifted_future.scaledBy(1 - EPSILON);
                    !m_pacman->isBlocked(getEntitiesInBounds(shifted_check))) {
                    // Apply the snap: Move Pacman to the aligned position immediately
                    m_pacman->setHitBox(shifted_current_hb);

                    // Unblock the move so the code below processes the direction change
                    lookaheadBlocked = false;
                    break;
                }
            }
        }

        // If still blocked after trying to corner cut, revert direction
        if (lookaheadBlocked) {
            d = current_direction;
        }
    }

    // Set the (potentially modified) direction
    m_pacman->setDirection(d);

    // --- Interaction Checks ---
    handleCollectables(current_hb);

    // --- Final Movement Calculation ---
    const Rectangle future_hb = IEntityModel::calculateFutureHitBox(current_hb, d, current_speed);

    if (const auto search_future_hb = future_hb.scaledBy(1 - EPSILON);
        !m_pacman->isBlocked(getEntitiesInBounds(search_future_hb))) {
        m_pacman->setHitBox(future_hb);
    } else {
        m_pacman->setDirection(Direction::Cardinal::NONE);
    }

    m_pacman->update(d);
}

void World::startPanic() const {
    for (const auto& ghost : m_ghosts) {
        ghost->setMode(GhostMode::PANICKING);
    }
}

void World::handleCollectables(const Rectangle& current_hb) {
    for (const auto& target_ptr : getEntitiesInBounds(current_hb.scaledBy(35.0 / 100.0))) {
        if (target_ptr.get() == m_pacman.get()) {
            continue;
        }
        CollisionHandler pacmanInitiates(*m_pacman);
        target_ptr->accept(pacmanInitiates);

        if (const auto result = pacmanInitiates.getResult();
            result == CollisionResult::COIN_PICKED_UP || result == CollisionResult::FRUIT_PICKED_UP) {
            CollectableVisitor pickup;
            if (result == CollisionResult::FRUIT_PICKED_UP) {
                startPanic();
            }
            target_ptr->markForDeletion();
            target_ptr->accept(pickup);
            if (result == CollisionResult::FRUIT_PICKED_UP) {
                startPanic();
            }
            target_ptr->markForDeletion();
        }

        CollisionHandler targetInitiates(*target_ptr);
        m_pacman->accept(targetInitiates);

        if (targetInitiates.getResult() == CollisionResult::GHOST_TOUCH) {
            if (const auto ghost = std::static_pointer_cast<IGhost>(target_ptr);
                ghost->getMode() == GhostMode::PANICKING) {
                ghost->die();
            } else {
                m_pacman->ghostTouches();
            }
        }
    }
}

WorldState World::getState() const { return m_worldState; }
