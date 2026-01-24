#include <iostream>
void swap(int& x, int& y) noexcept {
    int temp = x;
    x = y;
    y = temp;
}

int mul(int smallest, int biggest) {
    if (smallest > biggest) {
        swap(smallest, biggest);
    }
    int out = 0;
    for (int i = 0; i < smallest; i++) {
        out += biggest;
    }
    return out;
}

int main() {
    int val1, val2;
    std::cin >> val1;
    std::cin >> val2;
    std::cout << mul(val1, val2) << std::endl;
}