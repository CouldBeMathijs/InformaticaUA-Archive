//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_POTIONITEM_H
#define W13WOE_POTIONITEM_H


#include "Item.h"

class PotionItem : public Item{
private:
    int healthBoost;
public:
    PotionItem(const string &name, int healthBoost);

    ~PotionItem() override;

    string toString() const override;

    int getHealth_boost() const;

};


#endif //W13WOE_POTIONITEM_H
