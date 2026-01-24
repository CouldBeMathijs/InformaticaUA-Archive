//
// Created by mathijs on 12/18/24.
//

#ifndef W13WOE_ITEM_H
#define W13WOE_ITEM_H

#include <string>

using namespace std;
class Item {
private:
    string name;
public:
    explicit Item(const string &name);

    virtual ~Item();

    virtual string toString() const;
};


#endif //W13WOE_ITEM_H
