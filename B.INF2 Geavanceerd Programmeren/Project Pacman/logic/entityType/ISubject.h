#ifndef PACMAN_SUBJECT_H
#define PACMAN_SUBJECT_H
#include "../patterns/IObserver.h"
#include "Direction.h"

#include <memory>
#include <vector>

class IEntityVisitor;

/**
 * @brief Virtual base class for all classes which need to have observers or handle visitors
 */
class ISubject {
    std::vector<std::weak_ptr<IObserver>> m_observers;

protected:
    ISubject() = default;

public:
    /**
     * @brief Virtual function base for double dispatch
     * @param visitor The visitor object after single dispatch
     */
    virtual void accept(IEntityVisitor& visitor) = 0;

    /**
     * @brief Calls update on all connected observers
     */
    virtual void notify();

    /**
     * @brief Overridable general update class
     */
    virtual void update(Direction::Cardinal);

    virtual ~ISubject() = default;

    /**
     * @brief Adds an observer to this subject
     */
    void addObserver(std::weak_ptr<IObserver>);
};

#endif // PACMAN_SUBJECT_H
