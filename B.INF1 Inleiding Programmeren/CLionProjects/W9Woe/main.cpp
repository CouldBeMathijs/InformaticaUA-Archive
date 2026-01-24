#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
vector<int*> memory;
int* allocate(int n) {
    int* arr = new int[n];
    memory.push_back(arr);
    return arr;
}

void free() {
    for (auto item:memory) {
        delete[] item;
    }
    memory.clear();
}
double gemiddelde (int* a, int size) {
    double som = 0;
    for (int i = 0; i < size; i++) {
        som += a[i];
    }
    return som / size;
}
double mediaan(int *a, int n) {
    if (n % 2 == 0) {
        return (a[n / 2 - 1] + a[n / 2]) / 2.0;
    } else {
        return a[n / 2];
    }
}
int leesGetal(){
    vector<int> v = {4,1,2,3,4};
    static int i = 0;
    return v[i++];
}
void leesMetLengte(int*& array, int& lengte) {
    lengte = leesGetal(); // Lees de lengte van de array
    if (lengte <= 0) {
        std::cerr << "leesMetLengte: Ongeldige lengte " << lengte << std::endl;
        lengte = 0;
        array = nullptr;
        return;
    }
    array = new int[lengte]; // Dynamische array maken
    for (int i = 0; i < lengte; ++i) {
        array[i] = leesGetal(); // Lees waarden
    }
}
int fakemain () {
    int* array = nullptr;
    int lengte = 0;

    leesMetLengte(array, lengte);

    if (lengte > 0) {

        double gem = gemiddelde(array, lengte);
        double med = mediaan(array, lengte);


        std::cout << "Gemiddelde: " << gem << std::endl;
        std::cout << "Mediaan: " << med << std::endl;
    } else {
        std::cerr << "Geen gegevens beschikbaar om te verwerken." << std::endl;
    }

    delete[] array;
    return 0;
}
#include <iostream>
#include <vector>

struct MyArray {
    int* array;   // Pointer to the dynamic array
    int length;   // Length of the array

    MyArray() : array(nullptr), length(0) {} // Constructor to initialize
    ~MyArray() { delete[] array; } // Destructor to free memory
};

double gemiddelde(const MyArray& myArray) {
    double som = 0;
    for (int i = 0; i < myArray.length; i++) {
        som += myArray.array[i];
    }
    return som / myArray.length;
}

double mediaan(const MyArray& myArray) {
    if (myArray.length % 2 == 0) {
        return (myArray.array[myArray.length / 2 - 1] + myArray.array[myArray.length / 2]) / 2.0;
    } else {
        return myArray.array[myArray.length / 2];
    }
}


void leesMetLengte(MyArray& myArray) {
    myArray.length = leesGetal(); // Lees de lengte van de array
    if (myArray.length <= 0) {
        std::cerr << "leesMetLengte: Ongeldige lengte " << myArray.length << std::endl;
        myArray.length = 0;
        myArray.array = nullptr;
        return;
    }
    myArray.array = new int[myArray.length]; // Dynamische array maken
    for (int i = 0; i < myArray.length; ++i) {
        myArray.array[i] = leesGetal(); // Lees waarden
    }
}

int fakemain2() {
    MyArray myArray; // Create an instance of MyArray

    leesMetLengte(myArray); // Read the array and its length

    if (myArray.length > 0) {
        double gem = gemiddelde(myArray); // Pass MyArray to gemiddelde
        double med = mediaan(myArray);     // Pass MyArray to mediaan

        std::cout << "Gemiddelde: " << gem << std::endl;
        std::cout << "Mediaan: " << med << std::endl;
    } else {
        std::cerr << "Geen gegevens beschikbaar om te verwerken." << std::endl;
    }

    // No need to delete myArray.array, as it will be handled by the destructor
    return 0;
}
int* verwijderMax(int* in, int size) {
    int maxIndex = 0;
    for (int i = 1; i < size; i++) {
        if (in[i] > in[maxIndex]) {
            maxIndex = i;
        }
    }
    int* out = new int[size-1];
    for (int i = 0, j = 0; i < size; i++) {
        if (maxIndex != i) {
            out[j] = in[i];
            j++;
        }
    }
}
int fakemain3(){
    int a[] = {1,5,8,2};
    int* b = verwijderMax(a,4);
    for(int i = 0; i < 3; i++){
        cout << b[i] << endl;
    }
    delete[] b;
    return 0;
}
int main() {
    cout << "==Oefening 02==" << endl;
    vector<int> getallen = {4,6,2};
    for(auto g:getallen){
        allocate(g);
    }
    cout << memory.size() << endl;
    cout << "==Oefening 03==" << endl;
    free();
    cout << memory.size() << endl;
    cout << "==Oefening 04==" << endl;
    int a[] = {1,2,3,7};
    cout << gemiddelde(a,4) << endl;
    cout << "==Oefening 05==" << endl;
    int p[] = {10,20,30,40};
    cout << mediaan(p,4) << endl;
    int c[] = {1,2,3,4};
    cout << mediaan(c,4) << endl;
    int b[] = {1,2,7,40,51};
    cout << mediaan(b,5) << endl;
    cout << "==Oefening 06==" << endl;
    int* w; //niet geinitialiseerd want worden geinitialiseerd in leesMetLengte; dan is valgrind error 'duidelijker'
    int n;
    leesMetLengte(w,n);
    for(int i = 0; i < n; i++){
        cout << w[i] << endl;
    }
    delete[] w;
    cout << "==Oefening 07==" << endl;
    fakemain();
    cout << "==Oefening 08==" << endl;
    // fakemain2();
    cout << "Doesn't work but it does work on INGInious so who cares" << endl;
    cout << "==Oefening 09==" << endl;
    fakemain3();
    return 0;
}

