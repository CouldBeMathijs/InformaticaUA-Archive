#include "PausedState.h"

#include "../AssetManager.h"
#include "../Camera.h"

PausedState::PausedState() {
    const auto& window = Camera::getInstance().getWindow();
    const sf::Vector2u windowSize = window.getSize();

    m_pacmanText.setFont(Assets::getDefaultFont());
    m_pacmanText.setString("Paused");
    m_pacmanText.setCharacterSize(80);
    m_pacmanText.setFillColor(sf::Color::Yellow);

    const sf::FloatRect pacmanBounds = m_pacmanText.getLocalBounds();
    m_pacmanText.setOrigin(pacmanBounds.left + pacmanBounds.width / 2.0f,
                           pacmanBounds.top + pacmanBounds.height / 2.0f);

    m_pacmanText.setPosition(windowSize.x / 2.0f, windowSize.y / 4.0f);

    m_playText.setFont(Assets::getDefaultFont());
    m_playText.setString("Press Space to continue or Esc to exit");
    m_playText.setCharacterSize(30);
    m_playText.setFillColor(sf::Color::White);

    sf::FloatRect playBounds = m_playText.getLocalBounds();
    m_playText.setOrigin(playBounds.left + playBounds.width / 2.0f, playBounds.top + playBounds.height / 2.0f);

    m_playText.setPosition(windowSize.x / 2.0f, windowSize.y * 3 / 4.0f);
}

void PausedState::update(Direction::Cardinal) {
    auto& window = Camera::getInstance().getWindow();
    window.draw(m_pacmanText);
    window.draw(m_playText);
}

void PausedState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right ||
            event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
            return;
        }
        if (event.key.code == sf::Keyboard::Space) {
            m_requestedPops = 1;
        } else if (event.key.code == sf::Keyboard::Escape) {
            m_requestedPops = 2;
        }
    }
}
