//
// Created by Tim Apers on 10/01/2025.
//

#ifndef TILE_H
#define TILE_H

class ValueTile;
class WallTile;
class EmptyTile;

class Tile {
public:
    virtual ~Tile() = default;

    virtual char visualize() const;

    virtual bool replaceBy(Tile* other) = 0;
    virtual bool handleReplaceBy(ValueTile* other);
    virtual bool handleReplaceBy(WallTile* other);
    virtual bool handleReplaceBy(EmptyTile* other);
};



#endif //TILE_H
