//
// Created by mathijs on 12/18/24.
//

#include "PotionItem.h"

PotionItem::PotionItem(const string &name, int healthBoost) : Item(name), healthBoost(healthBoost) {}

PotionItem::~PotionItem() {

}

string PotionItem::toString() const {
    return Item::toString() + '\n' + "This item changes the health score by: " + to_string(healthBoost);
}

int PotionItem::getHealth_boost() const {
    return healthBoost;
}
