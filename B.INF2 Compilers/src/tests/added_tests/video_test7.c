// test7_semantic_errors.c
// LET OP: compileer dit bestand om je Error Analysis te testen!

// Forward declaration met mismatch in parameters (Project 5)
int my_function(int a);

// Definitie komt niet overeen met forward declaration
int my_function(float a) {
    return a;
}

int main() {
    /* 1. Gebruik van niet-gedeclareerde variabele */
    undeclared_var = 10;

    /* 2. Herdeclaratie van een variabele in dezelfde scope */
    int a = 5;
    int a = 10;

    /* 3. Toewijzing aan een const variabele */
    const int c = 100;
    c = 200;

    /* 4. Toewijzing aan een rvalue */
    5 = a;
    (a + 2) = 10;

    /* 5. Incompatibele types (Project 2 & 3) */
    int arr[5];
    arr = a; // Kan geen int toewijzen aan een heel array-type

    /* 6. Foutieve indexering van arrays */
    float index = 2.5;
    arr[index] = 10; // Array index moet een int zijn

    /* 7. Consistency van return types (Project 5) */
    return 3.14; // main moet een int returnen, geen float
}
