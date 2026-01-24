
#include "IEntityModel.h"

#include "../patterns/Visitor.h"

Rectangle IEntityModel::getHitBox() const { return m_hitBox; }

Direction::Cardinal IEntityModel::getDirection() const { return m_direction; }

IEntityModel::IEntityModel(Rectangle hitBox, const Direction::Cardinal d)
    : m_direction(d), m_hitBox(std::move(hitBox)) {}

bool IEntityModel::isInBounds(const Rectangle& boundBox) const {
    const bool x_overlap = m_hitBox.bottomRight.x > boundBox.topLeft.x && m_hitBox.topLeft.x < boundBox.bottomRight.x;
    const bool y_overlap = m_hitBox.bottomRight.y > boundBox.topLeft.y && m_hitBox.topLeft.y < boundBox.bottomRight.y;
    return x_overlap && y_overlap;
}

bool IEntityModel::isBlocked(const std::vector<std::shared_ptr<IEntityModel>>& touchingEntities) {
    const auto entity = this;
    for (const auto& target_ptr : touchingEntities) {

        if (target_ptr.get() == entity) {
            continue;
        }

        CollisionHandler entityInitiates(*entity);
        target_ptr->accept(entityInitiates);
        if (entityInitiates.getResult() == CollisionResult::MOVE_BLOCKED) {
            return true;
        }

        CollisionHandler targetInitiates(*target_ptr);
        entity->accept(targetInitiates);
        if (targetInitiates.getResult() == CollisionResult::MOVE_BLOCKED) {
            return true;
        }
    }
    return false;
}

Rectangle IEntityModel::calculateFutureHitBox(const Rectangle& current_hb, const Direction::Cardinal d,
                                              const double distance) {
    Rectangle future_hb = current_hb;

    switch (d) {
    case Direction::Cardinal::SOUTH:
        // Move the rectangle down (positive Y)
        future_hb.moveBy(0.0, distance);
        break;

    case Direction::Cardinal::WEST:
        // Move the rectangle left (negative X)
        future_hb.moveBy(-distance, 0.0);
        break;

    case Direction::Cardinal::NORTH:
        // Move the rectangle up (negative Y)
        future_hb.moveBy(0.0, -distance);
        break;

    case Direction::Cardinal::EAST:
        // Move the rectangle right (positive X)
        future_hb.moveBy(distance, 0.0);
        break;
        // Assuming other directions (like NONE) result in no movement.
    default:
        break;
    }

    return future_hb;
}

void IEntityModel::snapToGrid() { m_hitBox.snapToGrid(); }

bool IEntityModel::isMarkedForDeletion() const { return m_isMarkedForDeletion; }

void IEntityModel::markForDeletion() { m_isMarkedForDeletion = true; }