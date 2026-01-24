//
// Created by mathijs on 12/18/24.
//

#include <iostream>
#include "Level.h"
#include "Room.h"
Level::~Level() {
    for(auto room: rooms) {
        delete room;
    }
   // cout << "Deleted level" << endl;
};

Level::Level(const string &name) : name(name) {
    rooms = {nullptr,nullptr,nullptr,nullptr,nullptr};
}

void Level::addRoom(int x, Room *room) {
    rooms[x] = room;
}

Room *Level::getRoom(int x) {
    return rooms[x];
}
