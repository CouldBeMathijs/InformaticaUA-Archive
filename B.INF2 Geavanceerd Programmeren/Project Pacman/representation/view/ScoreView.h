/**
 * @file ScoreView.h
 * @brief UI component for rendering the player's score and life count.
 */

#ifndef PACMAN_SCORE_VIEW_H
#define PACMAN_SCORE_VIEW_H

#include "../../logic/patterns/IObserver.h"
#include <SFML/Graphics.hpp>

/**
 * @class ScoreView
 * @brief An observer dedicated to updating the on-screen HUD.
 * * This class tracks changes in the game's scoring logic and player stats.
 * When notified of a change, it retrieves the latest data from the ScoreKeeper
 * and updates the text strings for the score and lives display.
 */
class ScoreView final : public IObserver {
    sf::Text m_livesText; /**< Graphical text representing remaining lives (e.g., "Lives: 3"). */
    sf::Text m_scoreText; /**< Graphical text representing the current score (e.g., "Score: 1250"). */

public:
    /**
     * @brief Initializes the HUD text elements.
     * * Sets up fonts, character sizes, colors, and screen positions for the
     * score and lives display.
     */
    ScoreView();

    /**
     * @brief Refreshes the HUD text values.
     * * Triggered whenever the observed state changes (e.g., a pellet is eaten
     * or a life is lost). It pulls values directly from the ScoreKeeper singleton.
     */
    void update() override;
};

#endif // PACMAN_SCORE_VIEW_H