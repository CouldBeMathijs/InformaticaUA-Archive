//
// Created by Tim Apers on 10/01/2025.
//

#ifndef WALLTILE_H
#define WALLTILE_H

#include "Tile.h"

class WallTile : public Tile {
public:
    bool replaceBy(Tile *other) override;
};



#endif //WALLTILE_H
