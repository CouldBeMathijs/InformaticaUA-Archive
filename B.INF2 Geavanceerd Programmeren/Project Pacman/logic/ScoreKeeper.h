/**
 * @file ScoreKeeper.h
 * @brief Management of game statistics, scoring logic, and high score persistence.
 */

#ifndef PACMAN_SCOREKEEPER_H
#define PACMAN_SCOREKEEPER_H

#include "patterns/IObserver.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

/**
 * @struct HighScoreEntry
 * @brief Data structure representing a saved score and the date it was achieved.
 */
struct HighScoreEntry {
    unsigned int score;    /**< The final score value. */
    std::time_t timestamp; /**< The time the score was recorded. */

    /** @brief Used for sorting high scores in descending order. */
    bool operator>(const HighScoreEntry& other) const { return score > other.score; }
};

/**
 * @brief Utility to convert a time_t object into a human-readable string.
 * @param t The timestamp to format.
 * @return A string in "YYYY-MM-DD HH:MM" format.
 */
inline std::string formatTimestamp(const std::time_t t) {
    const std::tm* tm_ptr = std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
    return ss.str();
}

/**
 * @class ScoreKeeper
 * @brief Singleton responsible for tracking points, lives, levels, and persistent high scores.
 * * Implements the IObserver interface to respond to game-state changes (e.g., entity collisions).
 * It features a time-based multiplier and automatic score decay logic.
 */
class ScoreKeeper final : public IObserver {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

    // --- Member Variables ---
    TimePoint m_lastDeductionTime; /**< Last time the score decay was applied. */
    TimePoint m_lastPickupTime;    /**< Last time a collectable was picked up (for multipliers). */
    const std::string m_filePath = "./assets/.highscores"; /**< Storage location for high scores. */

    std::vector<HighScoreEntry> m_highScores;
    unsigned int m_collectablesLeft = 0;
    unsigned int m_currentScore = 0;
    unsigned int m_level = 1;
    unsigned int m_lives = 3;

    // --- Constants ---
    static constexpr double MAX_MULTIPLIER_TIME_S = 3.0;         /**< Window in seconds to maintain a combo. */
    static constexpr double TIME_BETWEEN_SCORE_DECREASE_S = 0.5; /**< Interval for periodic score deduction. */
    static constexpr int MAX_HIGH_SCORES = 5;                    /**< Number of entries to keep in the leaderboard. */

    /** @brief Private constructor for Singleton pattern. */
    ScoreKeeper();

public:
    // Delete copy/assignment to ensure a single instance.
    ScoreKeeper(const ScoreKeeper&) = delete;
    ScoreKeeper& operator=(const ScoreKeeper&) = delete;

    /** @return A reference to the list of top high scores. */
    const std::vector<HighScoreEntry>& getHighScores() { return m_highScores; };

    /** @return The global ScoreKeeper instance. */
    static ScoreKeeper& getInstance();

    /** @brief Checks if there are collectables (dots/pellets) remaining on the map. */
    [[nodiscard]] bool collectablesLeft() const;

    /** @brief Reads high scores from the local file into memory. */
    void loadHighScores();

    /** @brief Writes current high score list to the local file. */
    void saveHighScores() const;

    /** @brief Adds the current score to the leaderboard if it qualifies. */
    void recordCurrentScore();

    [[nodiscard]] unsigned int getLevel() const;
    [[nodiscard]] unsigned int getLives() const;
    [[nodiscard]] unsigned int getScore() const;

    /** @brief Increments the counter of collectables currently in the level. */
    void addCollectable();

    /**
     * @brief Adds points adjusted by how recently the last item was picked up.
     * @param baseScore The raw point value of the item.
     */
    void addPointsWithMultiplier(unsigned int baseScore);

    /** @brief Directly adds (or subtracts) a value from the current score. */
    void addToScore(int s);

    /** @brief Advances the game level and resets necessary state variables. */
    void nextLevel();

    /** @brief Decrements collectables; usually called when Pac-Man eats a pellet. */
    void removeCollectable();

    /** @brief Reduces the player's life count. */
    void removeLife();

    /** @brief Resets all session data (score, lives, level) to default. */
    void reset();

    /**
     * @brief Observer update hook.
     * Handles periodic logic like score deduction over time.
     */
    void update() override;
};

#endif // PACMAN_SCOREKEEPER_H