//
// Created by Tim Apers on 10/01/2025.
//

#include "WallTile.h"

bool WallTile::replaceBy(Tile* other) {
    return other->handleReplaceBy(this);
}