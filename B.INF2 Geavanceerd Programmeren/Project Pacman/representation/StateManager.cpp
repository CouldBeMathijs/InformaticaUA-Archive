#include "StateManager.h"

#include "Camera.h"
#include "state/MenuState.h"

StateManager::StateManager() { emplace(std::make_unique<MenuState>()); }

void StateManager::update(Direction::Cardinal d) {
    this->top()->update(d);
    const unsigned int requestedPops = this->top()->getRequestedPops();
    std::unique_ptr<IState> stateToPush;
    if (this->top()->isRequestedState()) {
        stateToPush = (this->top()->getRequestedState());
    }
    for (unsigned int i = 0; i < requestedPops; i++) {
        pop();
    }
    if (stateToPush) {
        this->push(std::move(stateToPush));
    }
}

void StateManager::handleInput(const sf::Event& event) { this->top()->handleInput(event); }

bool StateManager::empty() const { return std::stack<std::unique_ptr<IState>>::empty(); }
