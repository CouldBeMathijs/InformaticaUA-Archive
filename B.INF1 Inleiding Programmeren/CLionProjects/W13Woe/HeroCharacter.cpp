//
// Created by mathijs on 12/18/24.
//

#include "HeroCharacter.h"
#include "PotionItem.h"
#include "WeaponItem.h"
string HeroCharacter::toString() const {
    string out = Character::toString() + '\n';
    if (potion_items.empty()) {
        out += this->getName() + " carries no potion items";

    } else {
        out += this->getName() + " carries the following potion items:" + '\n';
    }
    for (auto potion: potion_items) {
        out += potion->toString();
    }
    out += '\n';
    if (weapon_items.empty()) {
        out += this->getName() + " carries no weapon items";

    } else {
        out += this->getName() + " carries the following weapon items:" + '\n';
    }
    for (auto weapon: weapon_items) {
        out += weapon->toString();
    }
    return out;
}

HeroCharacter::HeroCharacter(const string &name, int attackPower, int defensePower, int healthScore,
                             const vector<PotionItem *> &potionItems, const vector<WeaponItem *> &weaponItems)
        : Character(name, attackPower, defensePower, healthScore), potion_items(potionItems),
          weapon_items(weaponItems) {}

HeroCharacter::~HeroCharacter() {
    for (PotionItem* potion: potion_items) {
        delete potion;
    }
    for (WeaponItem* weapon: weapon_items) {
        delete weapon;
    }
}

int HeroCharacter::getAttackPower() const {
    int out = Character::getAttackPower();
    for (auto& weapon: weapon_items) {
        out += weapon->getAttack_boost();
    }
    return out;
}

int HeroCharacter::getDefensePower() const {
    int out = Character::getDefensePower();
    for (auto& weapon: weapon_items) {
        out += weapon->getDefense_boost();
    }
    return out;
}

int HeroCharacter::getHealthScore() const {
    int out = Character::getHealthScore();
    for (auto& potion: potion_items) {
        out += potion->getHealth_boost();
    }
    return out;
}

void HeroCharacter::addWeaponItem(WeaponItem * wi) {
    weapon_items.push_back(wi);
}

void HeroCharacter::addPotionItem(PotionItem * pi) {
    potion_items.push_back(pi);
}
