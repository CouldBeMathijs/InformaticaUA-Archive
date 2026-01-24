//
// Created by Tim Apers on 10/01/2025.
//

#include "EmptyTile.h"

bool EmptyTile::replaceBy(Tile* other) {
    return other->handleReplaceBy(this);
}