/**
 * @file LevelState.h
 * @brief Main gameplay state handler, managing the world simulation and its visual observers.
 */

#ifndef PACMAN_LEVEL_STATE_H
#define PACMAN_LEVEL_STATE_H

#include "../ViewCompatibleEntityFactory.h"
#include "IState.h"
#include "World.h"

/**
 * @class LevelState
 * @brief Coordinates active gameplay between the logic world and the visual views.
 * * This state initializes the level, manages the entity factory, and monitors
 * the World for state changes (Victory/Loss).
 * * Transitions:
 * - On Victory: Pushes a VictoryState and pops itself.
 * - On Game Over: Pushes a GameOverState and pops itself.
 * - On Escape: Pushes a PauseState without popping itself (overlay).
 */
class LevelState final : public IState {
private:
    /** @brief The physics and logic engine of the game. */
    World m_world;

    /** @brief Responsible for creating logic entities paired with their visual components. */
    std::shared_ptr<ViewCompatibleEntityFactory> m_factory;

    /** * @brief List of visual observers (Views) that render the world entities.
     * * Each frame, these views are updated to reflect the state of the World.
     */
    std::vector<std::shared_ptr<IObserver>> m_viewVector;

public:
    /**
     * @brief Constructor that sets up the level, loads the map, and spawns entities.
     */
    LevelState();

    /**
     * @brief Advances the game world by one tick and updates all visual observers.
     * @param d The current direction input from the user (e.g., via keyboard).
     */
    void update(Direction::Cardinal d) override;

    /**
     * @brief Handles system events like closing the window or hitting the Escape key.
     * @param event The SFML event to process.
     */
    void handleInput(const sf::Event& event) override;
};

#endif // PACMAN_LEVEL_STATE_H