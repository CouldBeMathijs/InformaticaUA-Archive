/**
 * @file IState.h
 * @brief Abstract base class for the Game State machine.
 */

#ifndef PACMAN_I_STATE_H
#define PACMAN_I_STATE_H

#include "../../logic/entityType/Direction.h"
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * @class IState
 * @brief Interface for all game states (Menu, Play, Pause, etc.).
 * * This class provides the blueprint for how states update their logic,
 * handle user input, and communicate state transitions back to a
 * State Manager.
 */
class IState {
public:
    /** @brief Virtual destructor ensuring proper cleanup of derived states. */
    virtual ~IState() = default;

    /**
     * @brief Logic update called every frame.
     * @param direction The current primary input direction from the controller.
     */
    virtual void update(Direction::Cardinal direction) = 0;

    /**
     * @brief Event handling for window events (keypress, close, etc.).
     * @param event The SFML event to process.
     */
    virtual void handleInput(const sf::Event& event) = 0;

    /**
     * @brief Returns the number of states the manager should remove from the stack.
     * @return unsigned int Number of 'pop' operations requested.
     */
    [[nodiscard]] unsigned int getRequestedPops() const { return m_requestedPops; }

    /**
     * @brief Checks if this state has requested a transition to a new state.
     * @return true if a new state is waiting to be pushed.
     */
    [[nodiscard]] bool isRequestedState() const { return m_requestedState.get(); }

    /**
     * @brief Retrieves the new state to be pushed onto the stack.
     * * This transfers ownership of the unique_ptr to the State Manager.
     * @return std::unique_ptr<IState> The new state to transition to.
     */
    [[nodiscard]] std::unique_ptr<IState> getRequestedState() { return std::move(m_requestedState); }

protected:
    /** @brief Protected constructor to prevent direct instantiation of the interface. */
    IState() = default;

    /** * @brief Signal to the State Manager to pop current states.
     * * Example: Setting to 1 will close the current menu.
     */
    unsigned int m_requestedPops = 0;

    /** * @brief Stores the next state to be added to the game loop.
     * * Example: In MenuState, this would be assigned a pointer to PlayState.
     */
    std::unique_ptr<IState> m_requestedState = nullptr;
};

#endif // PACMAN_I_STATE_H