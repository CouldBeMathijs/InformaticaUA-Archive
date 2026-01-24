//
// Created by mathijs on 12/11/24.
//

#include "Stack.h"
#include <iostream>
using namespace std;
int Stack::pop() {
    int todelte;
    get(size-1, todelte);
    del(size-1);
    return todelte;
}

Stack::Stack(int capacity) : ArrayList(capacity) {}

void Stack::push(int item) {
    add(item);
}

void Stack::print() {
    ArrayList::print();
}

Stack::~Stack() {
    cout << "calling destructor Stack" << endl;
}
