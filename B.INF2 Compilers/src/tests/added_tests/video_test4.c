// test4_advanced_features_final.c
#include <stdio.h>

// 1. Preprocessor (Project 5)
#define MAX_VAL 100

// 2. Globale variabele (Project 5)
int global_counter = 0;

// 3. Basis Struct (Project 6)
struct Point {
    int x;
    int y;
};

// 4. Geneste Struct (Project 6 - Optioneel)
// Een struct die andere structs als waarde bevat.
struct Rectangle {
    struct Point top_left;
    struct Point bottom_right;
    int area;
};

// 5. Simpele union (Project 6 - Optioneel)
union DataValue {
    int as_int;
    float as_float;
};

// 6. Functie met geneste struct als argument (Project 5 & 6)
void print_rect(struct Rectangle* r) {
    printf("Top-Left: (%d, %d)\n", r->top_left.x, r->top_left.y);
    printf("Bottom-Right: (%d, %d)\n", r->bottom_right.x, r->bottom_right.y);
}

// 7. Recursieve functie (Project 5)
int countdown(int n) {
    if (n <= 0) return 0;
    global_counter = global_counter + 1;
    return countdown(n - 1);
}

int main() {
    // A. Werken met de Geneste Struct
    struct Rectangle rect;

    // Toegang tot geneste velden (Project 6)
    rect.top_left.x = 0;
    rect.top_left.y = 10;
    rect.bottom_right.x = 10;
    rect.bottom_right.y = 0;
    rect.area = 100;

    print_rect(&rect);

    // B. Werken met de Union
    union DataValue data;
    data.as_int = 1065353216; // Bit-representatie van 1.0f
    printf("Union als int: %d\n", data.as_int);
    printf("Union als float: %f\n", data.as_float);

    // C. Overige features
    countdown(5);
    printf("Eindstand globale teller: %d\n", global_counter);

    return 0;
}
