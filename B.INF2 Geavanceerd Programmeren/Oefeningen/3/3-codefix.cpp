#include <iostream>
#include <memory>

void print(std::unique_ptr<std::string> message) {
    std::cout << message << std::endl;
}

void print(std::weak_ptr<std::string>& message) {
    std::cout << message << std::endl;
}

void print(std::string&& message) {
    std::cout << message << std::flush;
}

int main() {
    std::string m0("Hello");
    std::unique_ptr<std::string> m1(m0);
    std::weak_ptr<std::string> m2;

    print(m1); // Find two different solutions for this!
    {
        auto m3 = std::make_shared<std::string>("World!");
        m2 = m3;
        print(m2);
    }
    print(m2);
    print("!\n");
    print(m0);
}