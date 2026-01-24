//
// Created by mathijs on 12/18/24.
//

#include <iostream>
#include "World.h"
#include "Level.h"
World::~World() {
    for (auto level: levels) {
        delete level;
    }
  //  cout << "Deleted world" << endl;
}

void World::addLevel(Level * newLevel) {
    levels.push_back(newLevel);
}

World::World(const string &name) : name(name) {}
