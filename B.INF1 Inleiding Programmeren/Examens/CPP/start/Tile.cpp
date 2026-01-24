//
// Created by Tim Apers on 10/01/2025.
//

#include "Tile.h"

char Tile::visualize() const {
    return '?';
}

bool Tile::handleReplaceBy(ValueTile* other) {
    return true;
}

bool Tile::handleReplaceBy(WallTile* other) {
    return true;
}

bool Tile::handleReplaceBy(EmptyTile* other) {
    return true;
}