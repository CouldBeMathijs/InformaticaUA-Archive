#include <iostream>
#include "ArrayList.h"
using namespace std;


/**
 * De functie maakt een lege lijst uit 'l' zonder het geheugen vrij te geven.
 * @param l
 */
void ArrayList::clearList(){
    size = 0;
    for(int i=0; i<capacity; i++){
        items[i] = 0;
    }
}

/**
 * Al het geheugen in lst wordt vrijgegeven
 * @param lst
 */

/**
 *
 * @param lst
 * @return true als lst leeg is
 */
bool ArrayList::isEmpty(){
    return size == 0; //size is hetzelfde als (*lst).size
}

/**
 *
 * @param lst
 * @return size van de meegegeven lst
 */
int ArrayList::getSize(){
    return size;
}

/**
 *
 * @param lst
 * @param index
 * @param newElement is het nieuwe element wat het element op 'index' moet vervangen
 * @return return true als de operatie succesvol was
 */
bool ArrayList::set(int index, int newElement){
    //extra check of index > size
    if(index >= size or index <0 ){
        return false;
    }
    else{
        items[index] = newElement;
        return true;
    }
}

/**
 *
 * @param lst
 * @param index
 * @param item: de waarde die op de gegeven index zit wordt in item gezet.
 * @return true als de operatie succesvol was
 */
bool ArrayList::get(int index,int& item){
    //extra check of index > size
    if(index >= size or index <0 ){
        return false;
    }
    else{
        item = items[index];
        return true;
    }
}


/**
 * Functie
 * @param lst
 * @param index
 * @return return true als de operatie succesvol was
 */
bool ArrayList::del(int index){
    if(index >= size or index <0 ){
        return false;
    }
    else{
        //all elements after the deleted index are now shifted to the left
        for(int i = index; i < size-1;i++){
            items[i] = items[i+1];
        }
        size--;
        return true;
    }
}


/**
 * Functie verdubbeld de capaciteit van een meegegeven ArrayList
 * @param lst: de lijst diens capaciteit moet worden verdubbeld
 */
void ArrayList::doubleCapacity(){
    //double size
    int* newItems = new int[capacity * 2];
    //copy content items to newItems
    for(int i=0; i<size; i++){
        newItems[i] = items[i];
    }
    //delete previous list
    delete[] items;
    //update new lst
    items = newItems;
    capacity = capacity * 2;
}


/**
 * Voegt het newItem aan het einde van de array list toe
 * @param lst
 * @param newItem: het nieuwe item dat aan het einde van lst toegevoed moet worden
 */
void ArrayList::add(int newItem){
    //checks if list has reached it capacity, if so it will double the capacity such that new item will fit in the list
    if(size == capacity){
        this->doubleCapacity();
    }
    //add element
    items[size] = newItem;
    //increment size
    size++;
}


/**
 * Voegt op een meegeven index het meegegeven item in de ArrayList toe
 * @param lst
 * @param index
 * @param newItem het nieuwe item dat op de meegegeven index van lst toegevoed moet worden
 */
void ArrayList::insert(int index, int newItem){
    //checks if list has reached it capacity, if so it will double the capacity such that new item will fit in the list
    if(size == capacity){
        this->doubleCapacity();
    }
    //"shift" items to the right > index, start from high to low
    for(int i=size - 1; i>=index; i--){
        items[i+1] = items[i];
    }
    items[index] = newItem;
    size +=1;
}


/**
 * Print de meegegeven ArrayList in een net formaat
 * @param lst
 */
void ArrayList::print(){
    cout << "[";
    for(int i=0; i < size; i++){
        cout << items[i];
        if(i != size -1)
            cout << ",";
    }
    cout << "]" << endl;
}

ArrayList::ArrayList(int capacity) {
    size = 0;
    this->capacity = capacity;
    this->items = new int[capacity];
}

ArrayList::~ArrayList() {
    cout << "calling destructor ArrayList" << endl;
    delete[] items;
}




