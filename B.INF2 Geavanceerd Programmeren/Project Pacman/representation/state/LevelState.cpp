#include "LevelState.h"

#include "../../logic/mapLoading/WorldAsciiGridParser.h"
#include "GameOverState.h"
#include "PausedState.h"
#include "ScoreKeeper.h"
#include "VictoryState.h"
#include "mapLoading/WorldCreator.h"

LevelState::LevelState() : m_viewVector({}) {
    m_factory = std::make_shared<ViewCompatibleEntityFactory>(m_viewVector);
    const WorldAsciiGridParser parser;
    m_world = WorldCreator::createWorld("./assets/worldmap", parser, m_factory);
}

void LevelState::update(const Direction::Cardinal d) {
    switch (m_world.getState()) {
    case WorldState::RUNNING:
        m_world.update(d);
        return;
    case WorldState::VICTORY:
        m_requestedPops = 1;
        m_requestedState = std::make_unique<VictoryState>(ScoreKeeper::getInstance().getLevel() + 1,
                                                          ScoreKeeper::getInstance().getScore());
        return;
    case WorldState::GAME_OVER:
        m_requestedPops = 1;
        m_requestedState = std::make_unique<GameOverState>(ScoreKeeper::getInstance().getScore());
    }
}

void LevelState::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        m_requestedState = std::make_unique<PausedState>();
    }
}
