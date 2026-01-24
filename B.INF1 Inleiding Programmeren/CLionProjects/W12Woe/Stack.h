//
// Created by mathijs on 12/11/24.
//

#ifndef W12WOE_STACK_H
#define W12WOE_STACK_H
#include "ArrayList.h"
class Stack : private ArrayList {
public:
    explicit Stack(int capacity);

    int pop();

    void push(int item);

    void print();

    ~Stack() override;
};


#endif //W12WOE_STACK_H
