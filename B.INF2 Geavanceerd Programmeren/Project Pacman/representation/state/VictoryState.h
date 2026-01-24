/**
 * @file VictoryState.h
 * @brief State handler for the level completion / victory screen.
 */

#ifndef PACMAN_VICTORY_STATE_H
#define PACMAN_VICTORY_STATE_H

#include "IState.h"
#include <SFML/Graphics.hpp>

/**
 * @class VictoryState
 * @brief Manages the UI and logic after a level is successfully completed.
 * * This state displays the completed level number and the current total score.
 * * Transitions:
 * - Any Key: Pushes a new LevelState (for the next level) and pops itself.
 */
class VictoryState final : public IState {
private:
    sf::Text m_victoryText;      /**< Header text (e.g., "LEVEL COMPLETE!"). */
    sf::Text m_pressAnyKeyText;  /**< Instruction for the user to continue. */
    unsigned int m_level;        /**< The level index that was just finished. */
    unsigned int m_currentScore; /**< The player's current accumulated score. */

public:
    /**
     * @brief Constructs the Victory screen with session data.
     * @param level The level number that was completed.
     * @param currentScore The player's score at the time of victory.
     */
    explicit VictoryState(unsigned int level, unsigned int currentScore);

    /**
     * @brief Processes frame-by-frame logic for the victory screen.
     * * Usually handles visual updates like text pulsing or "fireworks" effects.
     * @param d The current input direction (typically ignored in this state).
     */
    void update(Direction::Cardinal d) override;

    /**
     * @brief Waits for user confirmation to advance to the next level.
     * @param event The SFML event containing keyboard or system data.
     */
    void handleInput(const sf::Event& event) override;
};

#endif