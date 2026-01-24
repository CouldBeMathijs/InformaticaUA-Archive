#include "ScoreKeeper.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <fstream>
#include <functional>

ScoreKeeper::ScoreKeeper() : m_lastDeductionTime(Clock::now()), m_lastPickupTime(Clock::now()) { loadHighScores(); }

ScoreKeeper& ScoreKeeper::getInstance() {
    static ScoreKeeper instance;
    return instance;
}

bool ScoreKeeper::collectablesLeft() const { return m_collectablesLeft != 0; }

void ScoreKeeper::loadHighScores() {
    m_highScores.clear();
    std::ifstream file(m_filePath);
    unsigned int s;
    std::time_t t;

    while (file >> s >> t) {
        m_highScores.push_back({s, t});
    }

    // Sort and trim just in case the file was manually edited
    std::sort(m_highScores.begin(), m_highScores.end(), std::greater<HighScoreEntry>());
    if (m_highScores.size() > MAX_HIGH_SCORES)
        m_highScores.resize(MAX_HIGH_SCORES);
}

void ScoreKeeper::saveHighScores() const {
    std::ofstream file(m_filePath, std::ios::trunc);
    for (const auto& [score, timestamp] : m_highScores) {
        file << score << " " << timestamp << "\n";
    }
}

void ScoreKeeper::recordCurrentScore() {
    HighScoreEntry newEntry;
    newEntry.score = m_currentScore;
    newEntry.timestamp = std::time(nullptr); // Gets current Unix timestamp

    m_highScores.push_back(newEntry);

    // Sort by score (descending)
    std::sort(m_highScores.begin(), m_highScores.end(), std::greater<HighScoreEntry>());

    // Keep only the top 5
    if (m_highScores.size() > MAX_HIGH_SCORES) {
        m_highScores.pop_back();
    }

    saveHighScores();
}

unsigned int ScoreKeeper::getLevel() const { return m_level; }

unsigned int ScoreKeeper::getLives() const { return m_lives; }

unsigned int ScoreKeeper::getScore() const { return m_currentScore; }

void ScoreKeeper::addCollectable() { m_collectablesLeft++; }

void ScoreKeeper::addPointsWithMultiplier(const unsigned int baseScore) {
    const TimePoint currentTime = Clock::now();

    const std::chrono::duration<double> timeElapsed = currentTime - m_lastPickupTime;
    const double timeSinceLastPickup_s = timeElapsed.count();

    const double multiplierTerm = std::max(1.0, MAX_MULTIPLIER_TIME_S - timeSinceLastPickup_s);

    const double pointsToAddD = static_cast<double>(baseScore) * multiplierTerm;

    m_currentScore += static_cast<unsigned int>(std::round(pointsToAddD));

    m_lastPickupTime = currentTime;
}

void ScoreKeeper::addToScore(const int s) { m_currentScore += s; }

void ScoreKeeper::nextLevel() {
    addToScore(250);
    m_level++;
}

void ScoreKeeper::removeCollectable() {
    m_collectablesLeft != 0 ? m_collectablesLeft-- : throw std::runtime_error("Cannot remove collectable, none left");
}

void ScoreKeeper::removeLife() { m_lives--; }

void ScoreKeeper::reset() {
    recordCurrentScore();
    m_collectablesLeft = 0;
    m_currentScore = 0;
    m_level = 1;
    m_lives = 3;
}

void ScoreKeeper::update() {
    const TimePoint now = Clock::now();

    const std::chrono::duration<double> timeElapsed = now - m_lastDeductionTime;

    if (const double timeSinceLastDeduction_s = timeElapsed.count();
        timeSinceLastDeduction_s >= TIME_BETWEEN_SCORE_DECREASE_S) {
        if (m_currentScore > 0) {
            m_currentScore--;
        }

        const int deductionCount = static_cast<int>(timeSinceLastDeduction_s / TIME_BETWEEN_SCORE_DECREASE_S);

        const std::chrono::duration<double> timeToAdvance(TIME_BETWEEN_SCORE_DECREASE_S * deductionCount);

        m_lastDeductionTime += std::chrono::duration_cast<Clock::duration>(timeToAdvance);
    }
}