#include "ScoreView.h"

#include "../AssetManager.h"
#include "../Camera.h"
#include "../SfmlConstants.h"
#include "ScoreKeeper.h"

ScoreView::ScoreView() {
    auto& font = Assets::getDefaultFont();

    m_livesText.setCharacterSize(50);
    m_livesText.setFillColor(sf::Color::Yellow);
    m_livesText.setFont(font);

    m_scoreText.setCharacterSize(50);
    m_scoreText.setFillColor(sf::Color::Yellow);
    m_scoreText.setFont(font);
}

void ScoreView::update() {
    auto& viewport = Camera::getInstance().getViewport();
    auto& window = Camera::getInstance().getWindow();
    constexpr float margin = 20.f;

    const unsigned int currentScore = ScoreKeeper::getInstance().getScore();
    const std::string scoreString = "Score: " + std::to_string(currentScore);
    m_scoreText.setString(scoreString);

    const unsigned int currentLives = ScoreKeeper::getInstance().getLives();
    const std::string livesString = "# Lives Left: " + std::to_string(currentLives);
    m_livesText.setString(livesString);

    const sf::FloatRect boundsScore = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(boundsScore.left, boundsScore.top);
    const float scorePosX = -viewport.getSize().x + margin;
    const float scorePosY = SfmlConstants::VIEW_HEIGHT - boundsScore.height - margin;
    m_scoreText.setPosition(scorePosX, scorePosY);
    window.draw(m_scoreText);

    const sf::FloatRect boundsLives = m_livesText.getLocalBounds();
    m_livesText.setOrigin(boundsLives.left, boundsLives.top);
    const float livesPosX = SfmlConstants::VIEW_WIDTH - boundsLives.width - margin;
    const float livesPosY = SfmlConstants::VIEW_HEIGHT - boundsLives.height - margin;
    m_livesText.setPosition(livesPosX, livesPosY);
    window.draw(m_livesText);
}