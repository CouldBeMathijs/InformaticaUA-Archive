// test5_missing_and_optional_features.c

// 1. Include Guards (Project 5 - Optioneel)
#ifndef MY_TEST_HEADER_H
#define MY_TEST_HEADER_H

#include <stdio.h>
#include <stdlib.h>

// 2. Function Overloading (Project 5 - Optioneel)
int multiply(int x, int y) {
    return x * y;
}

float multiply(float x, float y) {
    return x * y;
}

// Struct voor dynamische allocatie
struct HeapData {
    int id;
    float measurement;
};

int main() {
    /* 3. Unaire operatoren en Bitwise NOT (Project 1 - Verplicht) */
    int a = +5;
    int b = -a;
    int bit_not = ~a;

    /* 4. Const casting (Project 2 - Optioneel) */
    const float pi_const = 3.1415;
    const float* pi_ptr = &pi_const;
    float* non_const_pi_ptr = pi_ptr; // Dit is de const cast
    *non_const_pi_ptr = 3.14;         // Pas de waarde aan via de non-const pointer

    /* 5. Dynamische Allocatie met malloc/free (Project 3 & 6 - Optioneel) */
    // A. Dynamische array van ints
    int* dynamic_array = malloc(10 * 4); // Ruimte voor 10 ints (gesimplificeerde grootte)
    dynamic_array[0] = 100;
    dynamic_array[9] = 200;
    free(dynamic_array);

    // B. Dynamisch gealloceerde struct
    struct HeapData* my_data = malloc(8); // Gealloceerd op de heap
    my_data->id = 1;
    my_data->measurement = 9.81;
    free(my_data);

    // C. Dynamisch gealloceerde string/char buffer
    char* string_buffer = malloc(54);

    /* 6. File I/O: fgets en fputs (Project 6 - Optioneel) */
    // Bestand schrijven

    FILE* f = fopen("test_output.txt", "w");
    fputs("Dit is een testbestand gegenereerd door de compiler.\n", f);
    fclose(f);

    // Bestand inlezen
    f = fopen("test_output.txt", "r");
    fgets(string_buffer, 54, f);
    printf("Gelezen tekst: %s", string_buffer);
    fclose(f);

    free(string_buffer);

    /* 7. Functie overloading testen */
    int res_int = multiply(2, 4);
    float res_float = multiply(1.5, 2.0);

    printf("multiply(2, 4) = %d\n", res_int);
    printf("multiply(1.5, 2.0) = %f\n", res_float);

    return 0;
}

#endif // MY_TEST_HEADER_H
