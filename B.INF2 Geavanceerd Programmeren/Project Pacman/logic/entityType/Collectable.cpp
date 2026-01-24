#include "Collectable.h"

#include "../patterns/Visitor.h"
#include "ScoreKeeper.h"

Collectable::Collectable(const Rectangle& pos, const Direction::Cardinal d) : IEntityModel(pos, d) {}

void Collectable::bePickedUp() {
    if (!m_isPickedUp) {
        auto& score = ScoreKeeper::getInstance();
        m_isPickedUp = true;
        score.addPointsWithMultiplier(m_value);
        score.removeCollectable();
        notify();
    }
}

void Collectable::update(const Direction::Cardinal direction) {
    if (m_isPickedUp) {
        return;
    }
    IEntityModel::update(direction);
}

void Collectable::setValue(const int i) { m_value = i; }

Coin::Coin(const Rectangle& pos) : Collectable(pos, Direction::Cardinal::EAST) { setValue(10); }

void Coin::accept(IEntityVisitor& visitor) { visitor.visit(*this); }

Fruit::Fruit(const Rectangle& pos) : Collectable(pos, Direction::Cardinal::EAST) { setValue(20); }

void Fruit::accept(IEntityVisitor& visitor) { visitor.visit(*this); }
