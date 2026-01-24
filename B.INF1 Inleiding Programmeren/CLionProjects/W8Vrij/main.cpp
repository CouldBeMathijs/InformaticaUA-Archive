#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// bereken de absolute waarde van a
void absvalue(int &a){
    if(a < 0){
        a = -a;
    }
}

void abs(int a[],int n){
    for(int i = 0; i < n; i++){
        absvalue(a[i]);
    }
}

void print1(int a[],int n){
    for(int i = 0; i < n; i++){
        cout << a[i] << endl;
    }
}
int computeDiscriminant(int a, int b, int c) {
    return b * b - 4 * a * c;
}
int solveQuadraticEquation(vector<int> abc, double&x1, double&x2){
    int &a = abc[0];
    int &b = abc[1];
    int &c = abc[2];
    int discriminant = computeDiscriminant(a, b, c);
    if (discriminant < 0) {
        return 0;
    }else if (discriminant == 0){
        x1 = (-b+sqrt(discriminant))/(2*a);
        return 1;
    } else {
        x1 = (-b+sqrt(discriminant))/(2*a);
        x2 = (-b-sqrt(discriminant))/(2*a);
        return 2;
    }
}
int solveQuadraticEquationByArray(int* abc, double* x1, double* x2){
    int &a = abc[0];
    int &b = abc[1];
    int &c = abc[2];
    int discriminant = computeDiscriminant(a, b, c);
    if (discriminant < 0) {
        return 0;
    }else if (discriminant == 0){
        *x1 = (-b+sqrt(discriminant))/(2*a);
        return 1;
    } else {
        *x1 = (-b+sqrt(discriminant))/(2*a);
        *x2 = (-b-sqrt(discriminant))/(2*a);
        return 2;
    }
}
vector<int> readNumbers(string bestand) {
    vector<int> outputvector;
    ifstream file(bestand);
    int huidigegetal;
    while(file >> huidigegetal) {
        outputvector.push_back(huidigegetal);
    }
    return outputvector;
}
void writeNumbersInReverse(string bestand, vector<int> getallen) {
    ofstream output{bestand};
    for(int i = 0; i < getallen.size(); i++) {
        output << getallen[getallen.size() - i - 1] << endl;
    }
}
void shallowcopy(int* data, int** pointers, int grootte) {
    for(int i = 0; i < grootte; i++) {
        pointers[i] = &data[i];
    }
}
int** getPointer(int** pointers, int grootte, int index) {
    if (index >= 0 && index < grootte) {
        return &pointers[index];
    } else {
        return nullptr;
    }
}
void print(int** pointers, int** start, int** end) {
    while(start <= end) {
        cout<<**start<<" ";
        start++;
    }
    cout<<endl;
}
void reversecopy(int* data, int** pointers, int grootte) {
    for(int i = 0; i < grootte; i++) {
        pointers[i] = &data[grootte - i - 1];
    }
}
int findMin(int** pointers, int start, int end) {
    int minIndex = start;
    for (int i = start; i < end; ++i) {
        if (*pointers[i] < *pointers[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}
void sortedcopy(int* data, int** pointers, int grootte) {
    shallowcopy(data, pointers, grootte);
    int minIndex;
    int* tempp;
    for(int i = 0; grootte>i;i++) {
        minIndex = findMin(pointers, i, grootte);
        tempp = pointers[i];
        pointers[i] = pointers[minIndex];
        pointers[minIndex] = tempp;
    }
}
int main(){
    cout<<"==Oefening 01=="<<endl;
    int a[]  = {1,-3,-6,2};
    abs(a,4);
    print1(a,4);
    cout<<"==Oefening 02=="<<endl;
    cout<<computeDiscriminant(3, 4, 5)<<endl;
    cout<<computeDiscriminant(1, -2, 1)<<endl;
    cout<<computeDiscriminant(10, 5, 2)<<endl;
    cout<<computeDiscriminant(1, -2, -3)<<endl;
    cout<<"==Oefening 03=="<<endl;
    vector<int> abc1 = {1, -2, 1};
    vector<int> abc2 = {1, -2, -3};
    vector<int> abc3 = {10, 5, 2};
    double x1;
    double x2;
    cout<<solveQuadraticEquation(abc1, x1, x2)<<endl;
    cout<<x1<<endl;
    cout<<solveQuadraticEquation(abc2, x1, x2)<<endl;
    cout<<x1<<endl;
    cout<<x2<<endl;
    cout<<solveQuadraticEquation(abc3, x1, x2)<<endl;
    cout<<"==Oefening 04=="<<endl;
    cout<<"Works on INGInious, testing doesn't work because of previously declared values"<<endl;
    cout<<"==Oefening 05=="<<endl;
    vector<int> getallen = readNumbers("getallen.txt");
    for(auto i: getallen){
        cout << i << endl;
    }
    cout<<"==Oefening 06=="<<endl;
    getallen = {5,6,7,8,9};
    writeNumbersInReverse("getallenreverse.txt",getallen);
    vector<int> getallen2 = readNumbers("getallenreverse.txt");
    for(auto i: getallen2){
        cout << i << endl;
    }
    cout<<"==Oefening 07=="<<endl;
    // de basis
    int r[] = {1,-3,-6,2};

    // shallow copy
    int* s[]  = {nullptr, nullptr, nullptr, nullptr};
    shallowcopy(r,s,4);

    cout << *s[0] << endl; // geeft 1, het eerste element van de originele array

    // als je de originele aanpast, verandert de kopie ook (= shallow copy)
    r[0] = 5;
    cout << *s[0] << endl; // geeft dus nu 5
    cout<<"==Oefening 08=="<<endl;
    cout << **getPointer(s,4,0) << endl; // geeft dus nu 5
    cout<<"==Oefening 09=="<<endl;
    // de basis
    int q[]  = {1,-3,-6,2};

    // shallow copy
    int* w[]  = {nullptr, nullptr, nullptr, nullptr};
    shallowcopy(q,w,4);
    print(w, getPointer(w,4,0), getPointer(w,4,3));
    print(w, getPointer(w,4,1), getPointer(w,4,3));
    cout<<"==Oefening 10=="<<endl;
    reversecopy(q,w,4);
    print(w, getPointer(w,4,0), getPointer(w,4,3));
    print(w, getPointer(w,4,1), getPointer(w,4,3));
    cout<<"==Oefening 11=="<<endl;
    // min
    int minIndex = findMin(w, 0, 4);
    cout << "min zit op index " << minIndex << " = " << *w[minIndex] << endl;
    cout<<"==Oefening 12=="<<endl;
    sortedcopy(q,w,4);
    print(w, getPointer(w,4,0), getPointer(w,4,3)); // toont -6 -3 1 2
}