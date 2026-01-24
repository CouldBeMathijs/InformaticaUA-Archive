#ifndef PACMAN_STATE_MANAGER_H
#define PACMAN_STATE_MANAGER_H
#include "../logic/entityType/Direction.h"
#include "state/IState.h"

#include <memory>
#include <stack>

/**
 * @brief A stack of Abstract States which provides whatever the current top state requires of it
 */
class StateManager : std::stack<std::unique_ptr<IState>> {
public:
    StateManager();
    void update(Direction::Cardinal);
    void handleInput(const sf::Event&);
    [[nodiscard]] bool empty() const;
};

#endif
