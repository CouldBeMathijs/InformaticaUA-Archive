//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_WORLD_H
#define W13WOE_WORLD_H

#include <vector>
#include <string>

using namespace std;
class Level;

class World {
private:
    string name;
    vector<Level*> levels;
public:
    virtual ~World();
    void addLevel(Level*);

    explicit World(const string &name);
};


#endif //W13WOE_WORLD_H
