#include <iostream>
#include <string>
using namespace std;

struct Node {
    string item;
    Node *next;
    Node *prev;
};

struct LinkedList {
    int size;
    Node *head;
};

// Function to create a new linked list
LinkedList* createList() {
    auto* newlist = new LinkedList;
    newlist->size = 0;
    newlist->head = nullptr;
    return newlist;
}

// Check if the list is empty
bool isEmpty(LinkedList *list) {
    return list->head == nullptr;
}

// Get the length of the list
int getLength(LinkedList *list) {
    return list->size;
}

// Add a new item to the end of the list
void add(LinkedList *list, const string &newItem) {
    Node *newNode = new Node;
    newNode->item = newItem;
    newNode->next = nullptr;

    if (isEmpty(list)) {
        newNode->prev = nullptr;
        list->head = newNode;
    } else {
        Node *current = list->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
    list->size++;
}

// Get an item by index
string get(LinkedList *list, int index) {
    if (index < 0 || index >= list->size) {
        return "ERROR";
    }
    Node *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->item;
}

// Print the list
void print(LinkedList *list) {
    Node *current = list->head;
    while (current != nullptr) {
        cout << current->item << endl;
        current = current->next;
    }
}

// Set an item at a specific index
bool set(LinkedList * list, int index, const string & newItem) {
    if (index < 0 || index > list->size) {
        return false;
    }
    Node * newNode = new Node;
    newNode->item = newItem;
    if (index == 0) {
        newNode->prev = nullptr;
        newNode->next = list->head;
        if (list->head != nullptr) {
            list->head->prev = newNode;
        }
        list->head = newNode;
    } else {
        Node * current = list->head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        newNode->next = current->next;
        newNode->prev = current;
        if (current->next != nullptr) {
            current->next->prev = newNode;
        }
        current->next = newNode;
    }
    list->size++;
    return true;
}

// Remove an item at a specific index
bool remove(LinkedList *list, int index) {
    if (index < 0 || index >= list->size) {
        return false;
    }
    Node *toDelete;

    if (index == 0) {
        toDelete = list->head;
        list->head = list->head->next;
        if (list->head != nullptr) {
            list->head->prev = nullptr;
        }
    } else {
        Node *current = list->head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        toDelete = current->next;
        current->next = toDelete->next;
        if (toDelete->next != nullptr) {
            toDelete->next->prev = current;
        }
    }
    delete toDelete;
    list->size--;
    return true;
}

// Destroy the list and free memory
void destroyList(LinkedList *list) {
    Node *current = list->head;
    while (current != nullptr) {
        Node *toDelete = current;
        current = current->next;
        delete toDelete;
    }
    delete list;
}

int main() {
    auto l = createList();
    add(l, "Hello");
    add(l, "World");
    add(l, "!!");
    print(l);
    set(l, 1, "dear");
    print(l);
    remove(l, 3);
    print(l);
    destroyList(l);
    return 0;
}