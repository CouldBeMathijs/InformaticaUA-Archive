/**
 * @file MenuState.h
 * @brief State handler for the initial game menu and leaderboard display.
 */

#ifndef PACMAN_MENU_STATE_H
#define PACMAN_MENU_STATE_H

#include "../../logic/entityType/Direction.h"
#include "IState.h"
#include <vector>

/**
 * @class MenuState
 * @brief Manages the main menu interface, including title rendering and high scores.
 * * This state acts as the primary navigation layer.
 * * Transitions:
 * - Any Key (except Escape): Pushes a LevelState to begin the game.
 * - Escape: Requests a pop to exit the application.
 */
class MenuState final : public IState {
    sf::Clock m_refreshClock;               /**< Used for timing animations or UI polling. */
    sf::Text m_pacmanText;                  /**< The main game title/logo text. */
    sf::Text m_playText;                    /**< The "Press any key to start" prompt. */
    std::vector<sf::Text> m_highScoreTexts; /**< Visual list of top scores formatted for display. */

public:
    /**
     * @brief Constructor that initializes UI fonts, positions, and initial high score list.
     */
    MenuState();

    /**
     * @brief Updates menu logic, such as blinking text or refreshing high score data.
     * @param d The current input direction (typically unused in the menu).
     */
    void update(Direction::Cardinal d) override;

    /**
     * @brief Monitors for input to either start the game or quit.
     * @param event The SFML event containing keyboard/window signals.
     */
    void handleInput(const sf::Event& event) override;

    /**
     * @brief Fetches high scores from the ScoreKeeper and prepares the m_highScoreTexts vector.
     * * This is called during initialization and potentially when returning to the menu.
     */
    void setupHighScores();
};

#endif