//
// Created by Tim Apers on 10/01/2025.
//

#include "ValueTile.h"

bool ValueTile::replaceBy(Tile* other) {
    return other->handleReplaceBy(this);
}