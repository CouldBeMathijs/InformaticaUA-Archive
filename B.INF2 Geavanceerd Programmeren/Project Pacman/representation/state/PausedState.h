/**
 * @file PausedState.h
 * @brief State handler for the mid-game pause menu.
 */

#ifndef PACMAN_PAUSED_STATE_H
#define PACMAN_PAUSED_STATE_H

#include "../../logic/entityType/Direction.h"
#include "IState.h"

/**
 * @class PausedState
 * @brief Represents the paused state of the game session.
 * * This state is designed to be pushed onto the State Stack on top of a
 * LevelState. It halts gameplay logic and provides the user with options
 * to resume or quit to the menu.
 * * Transitions:
 * - Any Key (Except Escape): Resumes the game by popping itself.
 * - Escape Key: Requests multiple pops to return to the Main Menu.
 */
class PausedState final : public IState {
private:
    sf::Text m_pacmanText; /**< The "PAUSED" title text. */
    sf::Text m_playText;   /**< Instruction text (e.g., "Press any key to resume"). */

public:
    /**
     * @brief Constructor that initializes the pause menu UI elements.
     */
    PausedState();

    /**
     * @brief Update loop for the pause menu.
     * Note: Usually performs no logic as the game world is suspended.
     * @param direction The current input direction (ignored).
     */
    void update(Direction::Cardinal direction) override;

    /**
     * @brief Processes input to resume gameplay or exit to the menu.
     * @param event The SFML event to handle.
     */
    void handleInput(const sf::Event& event) override;
};

#endif