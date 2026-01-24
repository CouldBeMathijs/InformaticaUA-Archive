//
// Created by mathijs on 12/11/24.
//

#ifndef W12WOE_ARRAYLIST_H
#define W12WOE_ARRAYLIST_H


class ArrayList {
protected:
    int capacity;
    int size;
    int* items;
public:

    explicit ArrayList(int capacity);

    void clearList();

    bool isEmpty();

    int getSize();

    bool set(int index, int newElement);

    bool get(int index, int &item);

    bool del(int index);

    void doubleCapacity();

    void add(int newItem);

    void insert(int index, int newItem);

    void print();

    virtual ~ArrayList();
};


#endif //W12WOE_ARRAYLIST_H
