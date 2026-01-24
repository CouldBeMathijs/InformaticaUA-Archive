//
// Created by Tim Apers on 10/01/2025.
//

#ifndef EMPTYTILE_H
#define EMPTYTILE_H

#include "Tile.h"

class EmptyTile : public Tile {
public:
    bool replaceBy(Tile *other) override;
};



#endif //EMPTYTILE_H
