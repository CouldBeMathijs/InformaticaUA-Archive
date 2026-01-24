//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_TERRAINROOM_H
#define W13WOE_TERRAINROOM_H


#include "Room.h"

class TerrainRoom : public Room {
protected:
    string weatherDescription;
public:
    TerrainRoom(const string &description, const string &weatherDescription);

    string toString() const override;
};


#endif //W13WOE_TERRAINROOM_H
