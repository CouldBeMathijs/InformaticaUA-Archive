#include "Pacman.h"

#include "../patterns/Visitor.h"
#include "Stopwatch.h"

Pacman::Pacman(const Rectangle& pos)
    : IDirectionalEntityModel(pos, Direction::Cardinal::NONE, LogicConstants::BASE_SPEED) {}

void Pacman::goToSpawn() {
    IDirectionalEntityModel::goToSpawn();
    setDirection(Direction::Cardinal::NONE);
}

void Pacman::accept(IEntityVisitor& visitor) { visitor.visit(*this); }

void Pacman::ghostTouches() { m_ghostTouch = true; }

void Pacman::update(const Direction::Cardinal cardinal) {
    if (m_deathTimer > 0) {
        m_deathTimer -= Stopwatch::getInstance().getDeltaTime();
        if (m_deathTimer <= 0) {
            goToSpawn();
        }
    }
    IDirectionalEntityModel::update(cardinal);
}

void Pacman::resetGhostTouch() { m_ghostTouch = false; }

double Pacman::getDeathTimer() const { return m_deathTimer; }

void Pacman::die() {
    setDirection(Direction::Cardinal::NONE);
    m_deathTimer = LogicConstants::ANIMATION_SPEED * 10;
}

bool Pacman::hasTouchedGhost() const { return m_ghostTouch; }