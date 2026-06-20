#include <stdio.h>
#include "utils.h"

// 1. Recursie (Project 5)
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// 2. Overloading (Project 5 - Optioneel)
void print_val(int i) {
    printf("Integer waarde: %d\n", i);
}

void print_val(float f) {
    printf("Float waarde: %f\n", f);
}

int main() {
    print_header();

    // 3. Constant Folding & Propagation (Project 1 & 2)
    // De compiler moet dit intern reduceren
    int x = 10 + 5 * 2; // 20
    int y = x / 2;      // 10
    printf("Constant folding/prop resultaat (verwacht 10): %d\n", y);

    // 4. Structs & Typedefs (Project 6)
    Student s;
    s.id = 12345;
    s.grade = 'A';
    printf("Student %d heeft een %c\n", s.id, s.grade);

    // 5. Pointer rekenkunde op arrays (Project 2 & 3)
    int scores[5] = {10, 20, 30, 40, 50};
    int* p = scores;
    printf("Eerste score: %d, grootte van scores array: %d bytes\n", *p, sizeof(scores));
    
    int i;
    for (i = 0; i < 5; i++) {
        printf("Score %d: %d\n", i, *(p + i));
    }
    
    // 6. Recursie test
    int fib = fibonacci(6);
    printf("Fibonacci(6) resultaat (verwacht 8): %d\n", fib);

    // 7. Overloading test
    print_val(42);
    print_val(3.14);

    printf("--- Demo Succesvol Afgerond ---\n");
    return SUCCESS_CODE;
}
