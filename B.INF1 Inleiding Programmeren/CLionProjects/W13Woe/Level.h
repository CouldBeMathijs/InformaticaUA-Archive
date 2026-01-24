//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_LEVEL_H
#define W13WOE_LEVEL_H

#include <string>
#include <vector>
using namespace std;
class Room;
class Level {
private:
    string name;
    vector<Room*> rooms;
public:
    virtual ~Level();

    explicit Level(const string &name);

    void addRoom(int x, Room* room);

    Room* getRoom(int x);
};


#endif //W13WOE_LEVEL_H
