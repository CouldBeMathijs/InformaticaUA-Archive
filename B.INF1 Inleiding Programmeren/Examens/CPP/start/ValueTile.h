//
// Created by Tim Apers on 10/01/2025.
//

#ifndef VALUETILE_H
#define VALUETILE_H

#include "Tile.h"

class ValueTile : public Tile {
public:
    bool replaceBy(Tile *other) override;
};



#endif //VALUETILE_H
