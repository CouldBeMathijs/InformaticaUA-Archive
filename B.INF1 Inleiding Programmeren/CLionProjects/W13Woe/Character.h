//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_CHARACTER_H
#define W13WOE_CHARACTER_H

#include <string>

using namespace std;
class Character {
private:
    string name;
    int attackPower;
    int defensePower;
    int healthScore;
public:
    Character(const string &name, int attackPower, int defensePower, int healthScore);

    virtual ~Character();

    const string &getName() const;

    virtual int getAttackPower() const;

    virtual int getDefensePower() const;

    virtual int getHealthScore() const;

    virtual string toString() const;

    int getTotalPower() const;
};


#endif //W13WOE_CHARACTER_H
