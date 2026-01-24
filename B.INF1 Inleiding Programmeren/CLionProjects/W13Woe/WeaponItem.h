//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_WEAPONITEM_H
#define W13WOE_WEAPONITEM_H


#include "Item.h"

class WeaponItem : public Item{
private:
    int attackBoost;
    int defenseBoost;
public:
    WeaponItem(const string &name, int attackBoost, int defenseBoost);

    int getAttack_boost() const;

    int getDefense_boost() const;

    ~WeaponItem() override;

    string toString() const override;
};


#endif //W13WOE_WEAPONITEM_H
