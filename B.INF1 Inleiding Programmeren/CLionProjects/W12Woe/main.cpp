#include <iostream>
#include "ArrayList.h"
#include "Stack.h"
#include "Queue.h"
using namespace std;
int Oef1() {
    ArrayList l{3};
    l.add(203);
    l.print(); //Prints [203]
    l.add(123);
    l.print(); //prints [203,123]
    l.set(1,205);
    l.print(); //prints [203,205]
    int value;
    l.get(0,value);
    l.set(0, value - 1);
    l.print(); //prints [202,205]
    l.del(0);
    l.print(); //prints[205]
    l.clearList();
//system("python3 student/check_const_correctness.py student/student_labo01/ArrayList.cpp");
    return 0;
}
int Oef2() {
    Stack s{3};
    s.push(203);
    s.print();
    s.push(123);
    s.print();
    s.pop();
    s.push(205);
    s.print();
    int value = s.pop();
    s.print();
    return 0;
}
int Oef3(){
    Queue q{3};
    q.enqueue(203);
    q.print();
    q.enqueue(123);
    q.print();
    q.dequeue();
    q.enqueue(205);
    q.print();
    int value = q.dequeue();
    q.print();
    return 0;
}
#include "DenTweede.h"
#include "DenDerde.h"
#include "EnDeVierde.h"

class DenEerste: public DenTweede {
public:
    DenEerste(){
        DenDerde d;
        cout << "4, ";
    }

    virtual ~DenEerste(){
        cout << "6, ";
    }

private:
    EnDeVierde v;
};

int Oef4() {
    DenEerste d;
    return 0;
}
int main() {
    cout << endl << "==Oefening 1==" << endl;
    Oef1();
    cout << endl << "==Oefening 2==" << endl;
    Oef2();
    cout << endl << "==Oefening 3==" << endl;
    Oef3();
    cout << endl << "==Oefening 4==" << endl;
    Oef4();
}
