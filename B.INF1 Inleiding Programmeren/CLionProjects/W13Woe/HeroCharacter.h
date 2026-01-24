//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_HEROCHARACTER_H
#define W13WOE_HEROCHARACTER_H


#include <vector>
#include "Character.h"
#include "WeaponItem.h"

class PotionItem;

class HeroCharacter : public Character {
public:
    vector<PotionItem*> potion_items;
    vector<WeaponItem*> weapon_items;
public:
    HeroCharacter(const string &name, int attackPower, int defensePower, int healthScore,
                  const vector<PotionItem *> &potionItems, const vector<WeaponItem *> &weaponItems);

    ~HeroCharacter() override;

    int getAttackPower() const override;

    int getDefensePower() const override;

    int getHealthScore() const override;

    void addWeaponItem(WeaponItem*);

    void addPotionItem(PotionItem*);
    string toString() const override;


};


#endif //W13WOE_HEROCHARACTER_H
