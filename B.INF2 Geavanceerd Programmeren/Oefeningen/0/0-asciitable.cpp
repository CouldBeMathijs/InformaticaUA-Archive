#include <iostream>
int main() {
    std::cout << "ASCII TABLE" << std::endl;
    for (int i = 0; i <= 127; i++) {
        std::cout << i << "|" << static_cast<char>(i) << std::endl;
    }
}