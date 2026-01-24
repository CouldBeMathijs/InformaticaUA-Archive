//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_ROOM_H
#define W13WOE_ROOM_H
#include <string>
#include <vector>
#include "WeaponItem.h"

using namespace std;
class Item;
class PotionItem;
class HeroCharacter;
class Character;
class MonsterCharacter;

class Room {
protected:
    string description;
    vector<Item*> items;
    vector<Character*>characters;
public:
    explicit Room(const string &description);

    virtual ~Room();

    virtual string toString() const;

    void addItems(vector<Item*>);

    void addCharacters(vector<Character*>);

    void usePotion(HeroCharacter* hero, PotionItem* potion);

    void fight(HeroCharacter* hero, MonsterCharacter* monster);

    void pickUpWeapon(HeroCharacter *hero, WeaponItem *weapon);
};


#endif //W13WOE_ROOM_H
