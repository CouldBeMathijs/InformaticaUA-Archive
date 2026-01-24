//
// Created by mathijs on 12/18/24.
//

#include <iostream>
#include "TerrainRoom.h"

TerrainRoom::TerrainRoom(const string &description, const string &weatherDescription) : Room(description),
                                                                                        weatherDescription(
                                                                                                weatherDescription) {}

string TerrainRoom::toString() const {
    return Room::toString() + '\n' + "The weather conditions in this room are " + weatherDescription;

}
