#ifndef PACMAN_OBSERVER_H
#define PACMAN_OBSERVER_H

class ISubject;

/**
 * @brief Pure virtual base class for the observer pattern
 */
class IObserver {
protected:
    IObserver() = default;

public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

#endif // PACMAN_OBSERVER_H
