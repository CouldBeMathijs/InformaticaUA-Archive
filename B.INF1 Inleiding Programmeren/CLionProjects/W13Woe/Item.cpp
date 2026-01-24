//
// Created by mathijs on 12/18/24.
//

#include "Item.h"

Item::Item(const string &name) : name(name) {}

Item::~Item() {

}

string Item::toString() const {
    return "Item name: " + name;
}
