#ifndef W12WOE_QUEUE_H
#define W12WOE_QUEUE_H

#include "ArrayList.h"
class Queue : private ArrayList{
public:
    explicit Queue(int capacity);

    ~Queue() override;

    void enqueue(int item);

    int dequeue();

    void print();
};


#endif //W12WOE_QUEUE_H
