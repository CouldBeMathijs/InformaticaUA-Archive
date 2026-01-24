#include "Game.h"

#include "../logic/entityType/ISubject.h"
#include "Camera.h"
#include "Stopwatch.h"

#include <SFML/Graphics.hpp>

void Game::run() {
    auto& stopwatch = Stopwatch::getInstance();

    auto& camera = Camera::getInstance();
    sf::RenderWindow& window = camera.getWindow();

    auto direction = Direction::Cardinal::NONE;

    while (window.isOpen()) {
        stopwatch.tick();

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || m_sm.empty()) {
                window.close();
                return;
            }
            camera.handleEvent(event);
            m_sm.handleInput(event);
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Right:
                    direction = Direction::Cardinal::EAST;
                    break;
                case sf::Keyboard::Up:
                    direction = Direction::Cardinal::NORTH;
                    break;
                case sf::Keyboard::Left:
                    direction = Direction::Cardinal::WEST;
                    break;
                case sf::Keyboard::Down:
                    direction = Direction::Cardinal::SOUTH;
                    break;
                default:;
                }
            }
        }
        window.clear(sf::Color::Black);
        camera.resetView();
        m_sm.update(direction);
        if (m_sm.empty()) {
            window.close();
            return;
        }
        window.display();
    }
}