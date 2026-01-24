#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;
struct ArrayList {
    int size;
    int capacity;
    int* items;

};

ArrayList* createList(int initialCapacity) {
    ArrayList* newlist = new ArrayList;
    newlist->capacity = initialCapacity;
    newlist->size = 0;
    newlist->items = new int[initialCapacity];
    return newlist;
}
void clearList(ArrayList* array) {
    array->size = 0;
}
void destroyList(ArrayList* array) {
    delete[] array->items;
    delete array;
}
bool isEmpty(ArrayList* array) {
    return array->size == 0;
}
int getSize(ArrayList* array) {
    return array->size;
}
void print(ArrayList* array) {
    cout << "[";
    for (int i = 0; i < array->size; i++) {
        cout << array->items[i];
        if (i < array->size - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}
bool set(ArrayList* array, int index, int newItem) {
    if (index < 0 || index >= array->size) {
        return false;
    }
    array->items[index] = newItem;
    return true;
}
bool get(ArrayList* array, int index, int& item) {
    if (index < 0 || index >= array->size) {
        return false;
    }
    item = array->items[index];
    return true;
}
bool del(ArrayList* array, int index) {
    if (index < 0 || index >= array->size) {
        return false;
    }
    for (int i = index; i < array->size - 1; i++) {
        array->items[i] = array->items[i + 1];
    }
    array->size--;
    return true;
}
void doubleCapacity(ArrayList* array) {
    int newCapacity = array->capacity * 2;
    int* newItems = new int[newCapacity];
    for (int i = 0; i < array->size; i++) {
        newItems[i] = array->items[i];
    }
    delete[] array->items;
    array->items = newItems;
    array->capacity = newCapacity;
}
void add(ArrayList* array, int newItem) {
    if (array->size == array->capacity) {
        doubleCapacity(array);
    }
    array->items[array->size] = newItem;
    array->size++;
}
bool insert(ArrayList* array, int index, int newItem) {
    if (index < 0 || index > array->size) {
        return false;
    }
    if (array->size == array->capacity) {
        doubleCapacity(array);
    }
    for (int i = array->size; i > index; i--) {
        array->items[i] = array->items[i - 1];
    }
    array->items[index] = newItem;
    array->size++;
    return true;
}
ArrayList* loadList(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Fout bij openen van bestand: " << filename << endl;
        return nullptr;
    }
    ArrayList* list = new ArrayList;
    list->size = 0;
    list->capacity = 2;
    list->items = new int[list->capacity];
    int newItem;
    while (file >> newItem) {
        add(list, newItem);
    }
    file.close();
    return list;
}
void saveList(ArrayList* array, const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Fout bij openen van bestand: " << filename << endl;
        return;
    }
    for (size_t i = 0; i < array->size; ++i) {
        file << array->items[i];
        if (i < array->size - 1) {
            file << " ";
        }
    }
    file.close();
}
typedef ArrayList Stack;
Stack* createStack (){
    ArrayList* newlist = new ArrayList;
    newlist->capacity = 1;
    newlist->size = 0;
    newlist->items = new int[1];
    return newlist;
}
void push(Stack* stack, int newItem) {
    if (stack->size == stack->capacity) {
        doubleCapacity(stack);
    }
    stack->items[stack->size] = newItem;
    stack->size++;
}
void destroyStack(Stack* stack) {
    delete[] stack->items;
    delete stack;
}
bool pop(ArrayList* array) {
    int index = array->size - 1;
    if (index < 0 || index >= array->size) {
        return false;
    }
    for (int i = index; i < array->size - 1; i++) {
        array->items[i] = array->items[i + 1];
    }
    array->size--;
    return true;
}

int main() {
    // Run tests
    ArrayList* l = createList(3);
    cout << l->size << endl;
    cout << l->capacity << endl;
    for(int i = 0; i < l->capacity;i++){
        l->items[i] = 0;
        cout << l->items[i] << endl;
    }
    destroyList(l);
    cout << "==Stack==" << endl;
    Stack* s = createStack();
    push(s,203);
    print(s); //prints [203]
    push(s,99);
    print(s); //prints [203,99]
    int last = pop(s);
    print(s); //prints [203]
    destroyStack(s);
    return 0;
}