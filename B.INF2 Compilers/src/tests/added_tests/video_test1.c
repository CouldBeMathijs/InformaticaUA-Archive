int main() {
    /* 1. Basis expressies en operatoren (Project 1) */
    int a = 5 * (3 / 10 + 9 / 10);
    int b = a % 3;
    int c = (a > b) && (b != 0) || !(a == 5);

    // Bitwise en shift operatoren
    int bitwise = (a << 1) & 0xFF ^ 3 | 4;

    /* 2. Variabelen, Types en Literals (Project 2) */
    float f = 3.14;
    char ch = 'x';

    // Impliciete en expliciete conversies
    float implicit_cast = a;
    int explicit_cast = (int) f;

    /* 3. Pointers en Constanten */
    int z = 100;
    int* ptr = &z;
    int** ptr_to_ptr = &ptr; // Pointer to pointer

    const int const_val = 42;
    const int* const_ptr = &const_val; // Pointer naar een const

    /* 4. Pointer-aritmetiek en increment/decrement */
    int arr[5]; // Declaratie voor pointer rekenkunde
    int* p = arr;
    p = p + 2;
    p++;
    --p;
    int diff = p - arr;

    return 0;
}
