#include <iostream>

inline unsigned long int fib(const unsigned long int in) {
    unsigned long int out = 1;
    for (unsigned int i = 1; i <= in; i++) {
        out *= i;
    }
    return out;
}

unsigned long int fib_rec(unsigned long int in) {
    if (in <= 1) {
        return 1;
    }
    return in * fib_rec(in-1);
}

int main() {
    std::cout << "Give a number to get its factorial" << std::endl;
    int in;
    std::cin >> in;
    const auto out = fib_rec(in);
    //const auto out = fib_rec(in);
    std::cout << out << std::endl;
}
