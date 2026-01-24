//
// Created by mathijs on 12/18/24.
//

#include <iostream>
#include "Character.h"

const string &Character::getName() const {
    return name;
}

int Character::getAttackPower() const {
    return attackPower ;
}

int Character::getDefensePower() const {
    return defensePower;
}

int Character::getHealthScore() const {
    return healthScore;
}


string Character::toString() const {
    return name + " has the following characteristics:" + '\n' +
    "Attack Power: " + to_string(attackPower) + '\n' +
    "Defense Power: " + to_string(defensePower) + '\n' +
    "Health Score: " + to_string(healthScore);
}

Character::Character(const string &name, int attackPower, int defensePower, int healthScore) : name(name),
                                                                                               attackPower(attackPower),
                                                                                               defensePower(
                                                                                                       defensePower),
                                                                                               healthScore(
                                                                                                       healthScore) {}

Character::~Character() {
    //cout << "Deleted Character" << endl;
}

int Character::getTotalPower() const {
    return getAttackPower() + getDefensePower() + getHealthScore();
}
