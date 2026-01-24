#include <iostream>
#include <cstdlib>
using namespace std;
bool integerDivision(int teller, int noemer, int &quotient, int &rest) {
    if (noemer==0) {
        return false;
    }
    quotient = teller / noemer;
    rest = teller % noemer;
    return (rest == 0);
}
void add(int* a, int* b, int* result) {
    *result = *a + *b;
}
bool compareByValue(int* &p, int &i) {
    return *p == i;
}
bool compareByAddress(int* &p,int &i) {
    return p == &i;
}
void throwDice(int &d1, int &d2) {
    d1 = rand() % 6 + 1;
    d2 = rand() % 6 + 1;
}
void shiftLeft(int &a, int &b, int &c) {
    int temp = a;
    a = b;
    b = c;
    c = temp;
}
void shiftLeft2(int* a, int* b, int* c) {
    int temp = *a;
    *a = *b;
    *b = *c;
    *c = temp;
}
void print(char* a,int n) {
    for(int i=0; i < n; i++) {
        cout << a[i];
    }
    cout << endl;
}
void print2(char* a) {
    while ( *a != '\0') {
        cout << *a;
        a++;
    }
    cout << endl;
}
int telKlinkers(char* a) {
    int counter = 0;
    while (*a != '\0') {
        if (*a == 'a' || *a == 'e' || *a == 'i' || *a == 'o' || *a == 'u') {
            counter++;
        }
        a++;
    }
    return counter;
}

int main()
{
    std::cout << "==Oefening 01==" << std::endl;
    int teller1 = 18;
    int noemer1 = 9;
    int quotient = -1;
    int rest = -1;
    int noemer2 = 8;
    cout<<integerDivision(teller1, noemer1, quotient, rest)<<endl;
    cout<<quotient<<endl;
    cout<<rest<<endl;
    cout<<integerDivision(teller1, noemer2, quotient,rest)<<endl;
    cout<<quotient<<endl;
    cout<<rest<<endl;
    cout << "=Oefening 02" << endl;
    int a = 1;
    int b = 3;
    int c = 0;
    int d = 0;
    add(&a,&b,&c);
    cout << c << endl;
    add(&a,&c,&d);
    cout << d << endl;
    cout << "==Oefening 03" << endl;
    a = 5;
    int* q = &a;
    c = 5;
    d = 3;
    // boolalpha zal de waarde van de boolean afdrukken als true of false ipv 1 of 0
    cout << boolalpha << compareByValue(q,a) << endl; // geeft true
    cout << boolalpha << compareByValue(q,c) << endl; // geeft true
    cout << boolalpha << compareByValue(q,d) << endl; // geeft false
    cout << "==Oefening 04==" << endl;
    a = 5;
    q = &a;
    c = 5;
    d = 3;
    // boolalpha zal de waarde van de boolean afdrukken als true of false ipv 1 of 0
    cout << boolalpha << compareByAddress(q,a) << endl; // geeft true
    cout << boolalpha << compareByAddress(q,c) << endl; // geeft false
    cout << boolalpha << compareByAddress(q,d) << endl; // geeft false
    cout << "==Oefening 05==" << endl;
    int die1 = 0;
    int die2 = 0;
    for(int i = 0; i < 10; i++){
        throwDice(die1,die2);
        cout << "We gooien " << die1 << "," << die2 << "." << endl;
    }
    cout << "==Oefening 07==" << endl;
    a = 111;
    b = 32;
    c = 89;
    for(int i = 0; i < 10; i++){
        shiftLeft(a,b,c);
        cout << "(a,b,c) == (" << a << "," << b << "," << c << ")" << endl;
    }
    cout << "¨==Oefening 08==" << endl;
    for(int i = 0; i < 10; i++){
        shiftLeft2(&a,&b,&c);
        cout << "(a,b,c) == (" << a << "," << b << "," << c << ")" << endl;
    }
    cout << "==Oefening 08==" << endl;
    char zin[] = "Dit is geen zin. Het heeft immers geen zin, zo'n zin.";
    print(zin,53);
    cout << "==Oefening 09==" << endl;
    char blijf[] = "Dit is geen zin. Het heeft immers geen zin, zo'n zin.";
    print2(blijf);
    cout << "==Oefening 10" << endl;
    char testzin[] = "Dit is geen zin. Het heeft immers geen zin, zo'n zinnetje.";
    cout << telKlinkers(testzin) << endl;
    return 0;

}
