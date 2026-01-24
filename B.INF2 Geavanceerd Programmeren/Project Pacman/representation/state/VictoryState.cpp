#include "VictoryState.h"

#include "../AssetManager.h"
#include "../Camera.h"
#include "LevelState.h"
#include "ScoreKeeper.h"

/**
 * @brief Constructor for VictoryState.
 * Initializes and positions the 'VICTORY' and 'Press any key' text elements.
 */
VictoryState::VictoryState(const unsigned int level, const unsigned int currentScore)
    : m_level(level), m_currentScore(currentScore) {
    ScoreKeeper::getInstance().nextLevel();
    const auto& window = Camera::getInstance().getWindow();
    const sf::Vector2u windowSize = window.getSize();

    // --- Load Font ---
    // --- Setup 'VICTORY' Text ---
    m_victoryText.setFont(Assets::getDefaultFont());
    m_victoryText.setString("Level completed!");
    m_victoryText.setCharacterSize(80);
    // Use a celebratory color like Gold or Green
    m_victoryText.setFillColor(sf::Color(255, 215, 0)); // Gold

    // Center the 'VICTORY' text
    const sf::FloatRect victoryBounds = m_victoryText.getLocalBounds();
    m_victoryText.setOrigin(victoryBounds.left + victoryBounds.width / 2.0f,
                            victoryBounds.top + victoryBounds.height / 2.0f);

    // Position 'VICTORY' in the upper middle of the screen
    m_victoryText.setPosition(static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y) / 3.0f);

    // --- Setup 'Press Any Key' Text ---
    m_pressAnyKeyText.setFont(Assets::getDefaultFont());
    m_pressAnyKeyText.setString("Score: " + std::to_string(currentScore) + " - Press any key to start Level " +
                                std::to_string(level));
    m_pressAnyKeyText.setCharacterSize(30);
    m_pressAnyKeyText.setFillColor(sf::Color::White);

    // Center the 'Press Any Key' text
    const sf::FloatRect pressBounds = m_pressAnyKeyText.getLocalBounds();
    m_pressAnyKeyText.setOrigin(pressBounds.left + pressBounds.width / 2.0f,
                                pressBounds.top + pressBounds.height / 2.0f);

    // Position 'Press Any Key' in the lower middle of the screen
    m_pressAnyKeyText.setPosition(static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y) * 2 / 3.0f);
}

/**
 * @brief Updates the state, primarily used here for drawing the elements.
 * @param d The current direction (unused in this simple state).
 */
void VictoryState::update(Direction::Cardinal d) {
    auto& window = Camera::getInstance().getWindow();
    window.draw(m_victoryText);
    window.draw(m_pressAnyKeyText);
}

/**
 * @brief Handles user input.
 * Pops the current state (returns to the previous state on the stack) when any key is pressed.
 * @param event The SFML event that occurred.
 */
void VictoryState::handleInput(const sf::Event& event) {
    // Check if any key was pressed
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right ||
            event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
            return;
        }
        if (event.key.code == sf::Keyboard::Escape) {
            m_requestedPops = 1;
            return;
        }
        // Request to pop this state off the stack, returning to the previous state (MenuState)
        m_requestedPops = 1;
        m_requestedState = std::make_unique<LevelState>();
    }
}