#include <cmath>
#include <iostream>
#include <random>

double monteCarloPiRandomWalk(int walkLength, long long numWalks) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> coinToss(0, 1);

    double sumAbsolutePosition = 0.0;

    for (long long i = 0; i < numWalks; ++i) {
        int Wn = 0;

        for (int k = 0; k < walkLength; ++k) {
            int Xk = (coinToss(generator) * 2) - 1;

            Wn += Xk;
        }

        sumAbsolutePosition += std::abs(Wn);
    }

    double approximatedExpectation = sumAbsolutePosition / numWalks;

    double piEstimate = (2.0 * walkLength) / (approximatedExpectation * approximatedExpectation);

    return piEstimate;
}

int main() {
    const int N = 1000;
    const long long M = 5000000;
    std::cout << "Estimated value of Pi: " << monteCarloPiRandomWalk(N, M) << std::endl;
    return 0;
}