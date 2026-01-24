#include "GameOverState.h"

#include "../AssetManager.h"
#include "../Camera.h"
#include "ScoreKeeper.h"

/**
 * @brief Constructor for GameOverState.
 * Initializes and positions the 'Game Over' and 'Press any key' text elements.
 */
GameOverState::GameOverState(unsigned int finalScore) : m_finalScore(finalScore) {
    const auto& window = Camera::getInstance().getWindow();
    const sf::Vector2u windowSize = window.getSize();
    ScoreKeeper::getInstance().reset();

    // --- Setup 'Game Over' Text ---
    m_gameOverText.setFont(Assets::getDefaultFont());
    m_gameOverText.setString("Game Over");
    m_gameOverText.setCharacterSize(80);
    m_gameOverText.setFillColor(sf::Color::Red);

    // Center the 'Game Over' text
    const sf::FloatRect gameOverBounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width / 2.0f,
                             gameOverBounds.top + gameOverBounds.height / 2.0f);

    // Position 'Game Over' in the upper middle of the screen
    m_gameOverText.setPosition(windowSize.x / 2.0f, windowSize.y / 3.0f);

    // --- Setup 'Press Any Key' Text ---
    m_pressAnyKeyText.setFont(Assets::getDefaultFont());
    m_pressAnyKeyText.setString("Final Score: " + std::to_string(m_finalScore) + " - Press any key to return to Menu");
    m_pressAnyKeyText.setCharacterSize(30);
    m_pressAnyKeyText.setFillColor(sf::Color::White);

    // Center the 'Press Any Key' text
    sf::FloatRect pressBounds = m_pressAnyKeyText.getLocalBounds();
    m_pressAnyKeyText.setOrigin(pressBounds.left + pressBounds.width / 2.0f,
                                pressBounds.top + pressBounds.height / 2.0f);

    // Position 'Press Any Key' in the lower middle of the screen
    m_pressAnyKeyText.setPosition(windowSize.x / 2.0f, windowSize.y * 2 / 3.0f);
}

/**
 * @brief Updates the state, primarily used here for drawing the elements.
 * @param d The current direction (unused in this simple state).
 */
void GameOverState::update(Direction::Cardinal d) {
    auto& window = Camera::getInstance().getWindow();
    window.draw(m_gameOverText);
    window.draw(m_pressAnyKeyText);
}

/**
 * @brief Handles user input.
 * Pops the current state (returns to the previous state on the stack) when any key is pressed.
 * @param event The SFML event that occurred.
 */
void GameOverState::handleInput(const sf::Event& event) {
    // Check if any key was pressed
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code != sf::Keyboard::Left && event.key.code != sf::Keyboard::Right &&
            event.key.code != sf::Keyboard::Up && event.key.code != sf::Keyboard::Down) {
            // Request to pop this state only for non-arrow keys
            m_requestedPops = 1;
        }
    }
}