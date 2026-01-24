#include "ISubject.h"

#include <utility>

void ISubject::update(Direction::Cardinal) { notify(); }

void ISubject::addObserver(std::weak_ptr<IObserver> n) { m_observers.push_back(std::move(n)); }

void ISubject::notify() {
    for (auto it = m_observers.begin(); it != m_observers.end();) {
        // Try to promote weak_ptr to shared_ptr
        if (const auto observer = it->lock()) {
            observer->update();
            ++it;
        } else {
            // Optimization: If the observer is gone, remove it from the list
            it = m_observers.erase(it);
        }
    }
}