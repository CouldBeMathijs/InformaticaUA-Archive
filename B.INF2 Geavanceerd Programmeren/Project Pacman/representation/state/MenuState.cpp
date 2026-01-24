#include "MenuState.h"

#include "../AssetManager.h"
#include "../Camera.h"
#include "LevelState.h"
#include <iomanip>

MenuState::MenuState() {
    const auto& window = Camera::getInstance().getWindow();
    const sf::Vector2u windowSize = window.getSize();

    m_pacmanText.setFont(Assets::getDefaultFont());
    m_pacmanText.setString("Pacman");
    m_pacmanText.setCharacterSize(80);
    m_pacmanText.setFillColor(sf::Color::Yellow);
    const sf::FloatRect pacmanBounds = m_pacmanText.getLocalBounds();
    m_pacmanText.setOrigin(pacmanBounds.left + pacmanBounds.width / 2.0f,
                           pacmanBounds.top + pacmanBounds.height / 2.0f);
    m_pacmanText.setPosition(windowSize.x / 2.0f, windowSize.y / 4.0f);

    setupHighScores();

    m_playText.setFont(Assets::getDefaultFont());
    m_playText.setString("Press any key to Play");
    m_playText.setCharacterSize(30);
    m_playText.setFillColor(sf::Color::White);
    const sf::FloatRect playBounds = m_playText.getLocalBounds();
    m_playText.setOrigin(playBounds.left + playBounds.width / 2.0f, playBounds.top + playBounds.height / 2.0f);
    m_playText.setPosition(windowSize.x / 2.0f, windowSize.y * 0.85f);
    m_refreshClock.restart(); // Start the 10-second timer
}

void MenuState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            m_requestedPops = 1;
            return;
        }
        m_requestedState = std::make_unique<LevelState>();
    }
}

void MenuState::setupHighScores() {
    m_highScoreTexts.clear();

    // Access window and scores directly from singletons
    const auto& window = Camera::getInstance().getWindow();
    const auto& scores = ScoreKeeper::getInstance().getHighScores();

    float startY = window.getSize().y * 0.4f;

    for (size_t i = 0; i < scores.size(); ++i) {
        constexpr float spacing = 40.0f;
        sf::Text text;
        text.setFont(Assets::getDefaultFont());
        text.setCharacterSize(24);
        text.setFillColor(sf::Color(255, 255, 255, 220));

        // Format Date and Score
        std::tm* tm_ptr = std::localtime(&scores[i].timestamp);
        std::stringstream ss;
        ss << i + 1 << ". " << std::setw(6) << scores[i].score << "  [" << std::put_time(tm_ptr, "%Y-%m-%d") << "]";

        text.setString(ss.str());

        // Centering logic
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        text.setPosition(window.getSize().x / 2.0f, startY + (i * spacing));

        m_highScoreTexts.push_back(text);
    }
}

void MenuState::update(Direction::Cardinal d) {
    auto& window = Camera::getInstance().getWindow();

    // Rerun setup every 10 seconds
    if (m_refreshClock.getElapsedTime().asSeconds() >= 10.0f) {
        setupHighScores();
        m_refreshClock.restart();
    }

    // Render
    window.draw(m_pacmanText);
    for (const auto& hsText : m_highScoreTexts) {
        window.draw(hsText);
    }
    window.draw(m_playText);
}
