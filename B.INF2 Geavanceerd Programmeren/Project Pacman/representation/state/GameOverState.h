/**
 * @file GameOverState.h
 * @brief State handler for the Game Over screen.
 */

#ifndef PACMAN_GAME_OVER_STATE_H
#define PACMAN_GAME_OVER_STATE_H

#include "../../logic/entityType/Direction.h"
#include "IState.h"

/**
 * @class GameOverState
 * @brief Handles the logic and rendering of the end-game sequence.
 * * This state displays the player's final score and waits for a user
 * keypress to transition back to the main menu. It is an implementation
 * of the State Pattern (`IState`).
 */
class GameOverState final : public IState {
    const unsigned int m_finalScore; /**< The score achieved during the session. */

    // --- UI Elements (SFML) ---
    sf::Text m_gameOverText;    /**< Header text (e.g., "GAME OVER"). */
    sf::Text m_playText;        /**< Displays the final score value. */
    sf::Text m_pressAnyKeyText; /**< Instruction for the user to proceed. */

public:
    /**
     * @brief Processes frame-by-frame logic for the Game Over screen.
     * @param d The current input direction (usually ignored in this state).
     */
    void update(Direction::Cardinal d) override;

    /**
     * @brief Constructs the Game Over state.
     * @param finalScore The score to be displayed to the player.
     */
    explicit GameOverState(unsigned int finalScore);

    /**
     * @brief Monitors for a keypress to return to the menu.
     * @param event The SFML event containing keyboard or window data.
     */
    void handleInput(const sf::Event& event) override;
};

#endif