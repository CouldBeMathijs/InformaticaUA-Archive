#include "SpawnWall.h"

#include "../patterns/Visitor.h"

SpawnWall::SpawnWall(const Rectangle& hitBox) : IEntityModel(hitBox, Direction::Cardinal::EAST) {}

void SpawnWall::accept(IEntityVisitor& visitor) { visitor.visit(*this); }
