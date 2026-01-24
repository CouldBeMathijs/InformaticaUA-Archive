#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H
#include "StateManager.h"

/**
 * @brief Top level object in charge of holding the StateManager and main Game loop
 */
class Game {
    StateManager m_sm;

public:
    Game() = default;
    void run();
};

#endif // PACMAN_GAME_H
