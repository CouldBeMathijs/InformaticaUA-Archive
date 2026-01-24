//
// Created by mathijs on 12/11/24.
//

#include "Queue.h"
#include <iostream>
using namespace std;
Queue::Queue(int capacity) : ArrayList(capacity) {}

Queue::~Queue() {
cout << "calling destructor Queue" << endl;
}

void Queue::enqueue(int item) {
    add(item);
}

int Queue::dequeue() {
    int todelte;
    get(0, todelte);
    del(0);
    return todelte;
}

void Queue::print() {
    ArrayList::print();
}


