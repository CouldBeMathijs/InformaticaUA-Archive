//
// Created by mathijs on 12/18/24.
//

#include "Room.h"
#include "Character.h"
#include "Item.h"
#include "PotionItem.h"
#include "HeroCharacter.h"
#include "MonsterCharacter.h"
#include <iostream>
using namespace std;
Room::Room(const string &description) : description(description) {}

Room::~Room() {
    for(auto&item:items) {
        delete item;
    }
    for(auto&ch:characters) {
        delete ch;
    }
   // cout << "Deleted room" << endl;
}

string Room::toString() const {
    string out = "This is a room called '" + description + "'" + '\n' +
    "In this room there are the following characters: " + '\n';
    for (auto&ch:characters){
        out += ch->toString() + '\n';
    }
    for (auto&item:items){
        out += item->toString() + '\n';
    }
    return out;
}

void Room::addItems(vector<Item *>newItems) {
    for (auto newItem:newItems) {
        items.push_back(newItem);
    }
}

void Room::addCharacters(vector<Character *> newChars) {
    for (auto newChar:newChars) {
        characters.push_back(newChar);
    }
}

void Room::usePotion(HeroCharacter *hero, PotionItem *potion) {
    for (long i = 0; i < items.size();) {
        if (dynamic_cast<PotionItem*>(items[i]) == potion) {
            items.erase(items.begin() + i);
        } else {
            ++i;
        }
    }
    hero->addPotionItem(potion);
}
void Room::pickUpWeapon(HeroCharacter *hero, WeaponItem *weapon) {
    for (long i = 0; i < items.size();) {
        if (dynamic_cast<WeaponItem*>(items[i]) == weapon) {
            items.erase(items.begin() + i);
        } else {
            ++i;
        }
    }
    hero->addWeaponItem(weapon);
}

void Room::fight(HeroCharacter *hero, MonsterCharacter *monster) {
    if (hero->getTotalPower() > monster->getTotalPower()) {
        cout << "Hero wins!" << endl;
        for (long i = 0; i < characters.size();) {
            if (dynamic_cast<HeroCharacter*>(characters[i]) == hero) {
                characters.erase(characters.begin() + i);
            } else {
                ++i;
            }
        }
        return;
    } else if (hero->getTotalPower() < monster->getTotalPower()) {
        cout << "Monster wins!" << endl;
        for (long i = 0; i < characters.size();) {
            if (dynamic_cast<MonsterCharacter*>(characters[i]) == monster) {
                characters.erase(characters.begin() + i);
            } else {
                ++i;
            }
        }
        return;
    } else {
        cout << "They are each-others equals!" << endl;
    }
}




