#include <iostream>
#include <string>
using namespace std;
struct Node {
    string item;
    Node *next;
    Node *prev;
};
struct Queue {
    int size;
    Node *head;
};
Queue* createQueue() {
    auto* newlist = new Queue;
    newlist->size = 0;
    newlist->head = nullptr;
    return newlist;
}
bool isEmpty(Queue *queue) {
    return queue->head == nullptr;
}
bool enqueue(Queue *queue, const string &newItem) {
    Node *newNode = new Node;
    newNode->item = newItem;
    newNode->next = nullptr;

    if (isEmpty(queue)) {
        newNode->prev = nullptr;
        queue->head = newNode;
    } else {
        Node *current = queue->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
    queue->size++;
    return true;
}
string dequeue(Queue * queue) {
    if (queue->head == nullptr) {
        return "ERROR";
    }
    Node* toDelete = queue->head;
    string returnvalue = queue->head->item;
    queue->head = queue->head->next;
    delete toDelete;
    return returnvalue;
}
void print(Queue *queue) {
    Node *current = queue->head;
    while (current != nullptr) {
        cout << current->item << endl;
        current = current->next;
    }
}
void destroyQueue(Queue *queue) {
    Node *current = queue->head;
    while (current != nullptr) {
        Node *toDelete = current;
        current = current->next;
        delete toDelete;
    }
    delete queue;
}
int queuetest(){
    Queue* q = createQueue();
    cout << boolalpha << isEmpty(q) << endl;
    cout << dequeue(q) << endl;
    enqueue(q, "Jos");
    enqueue(q ,"An");
    enqueue(q, "Peter");
    print(q);             //Jos, An en Peter worden op drie regels geprint
    string first = dequeue(q);
    cout << first << endl;  //Jos wordt geprint
    print(q);           //An en Peter worden geprint
    destroyQueue(q);
    return 0;
}