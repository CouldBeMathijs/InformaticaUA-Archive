// test6_edge_cases.c
#include <stdio.h>

// Typedef voor ambiguïteitstest
typedef float MyFloat;

// Struct voor array-of-structs test
struct Config {
    int id;
    MyFloat threshold;
};

// Array doorgeven als functie-argument (Project 3 & 5)
int sum_array(int arr[], int size) {
    int sum = 0;
    int i;
    for(i = 0; i < size; i++) {
        sum = sum + arr[i];
    }
    return sum;
}

int main() {
    /* 1. Typedef Pointer Ambiguïteit (Project 6 - Verplicht)
       De parser ziet "MyFloat * ptr". Zonder goede symbol table
       denkt hij dat dit een vermenigvuldiging is in plaats van een declaratie! */
    MyFloat value = 3.14;
    MyFloat* ptr = &value;

    /* 2. Arrays van structs (Project 6 - Optioneel) */
    struct Config configs[3];
    configs[0].id = 1;
    configs[0].threshold = 0.5;

    configs[1].id = 2;
    configs[1].threshold = *ptr;

    /* 3. Arrays doorgeven aan functies */
    int my_numbers[4] = {10, 20, 30, 40};
    int total = sum_array(my_numbers, 4);

    return 0;
}
