//
// Created by Tim Apers on 10/01/2025.
//

#ifndef GAME_H
#define GAME_H

#include "Tile.h"
#include <map>

using namespace std;

class Game {

public:
    ~Game();
    void render();
    pair<int, int> quantizeDirection(pair<int, int> direction);
    void move(pair<int, int> direction);
    bool moveTile(pair<int, int> pos, pair<int, int> direction);


private:
    map<pair<int, int>, Tile*> tiles;

};



#endif //GAME_H
