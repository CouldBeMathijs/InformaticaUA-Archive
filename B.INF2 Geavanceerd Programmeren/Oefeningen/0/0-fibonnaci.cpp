#include <iostream>
int main() {
    std::cout << "Number of fibonnaci numbers to return: ";
    int fib_1 = 0;
    int fib_2 = 1;
    unsigned int max = 0;
    std::cin >> max;
    for (unsigned int _ = 0; _ < max; _++) {
        int fib_new = fib_1 + fib_2;
        std::cout << fib_new << "\n";
        fib_1 = fib_2;
        fib_2 = fib_new;
    }
    std::cout << std::endl;
}