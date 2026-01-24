//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_MONSTERCHARACTER_H
#define W13WOE_MONSTERCHARACTER_H


#include "Character.h"

class MonsterCharacter : public Character{
public:
    MonsterCharacter(const string &name, int attackPower, int defensePower, int healthScore);

    ~MonsterCharacter() override;

};


#endif //W13WOE_MONSTERCHARACTER_H
