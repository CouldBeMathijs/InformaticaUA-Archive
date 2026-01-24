#include "Random.h"

Random::Random() {
    std::random_device rd;
    m_generator = std::mt19937(rd());
}

Random& Random::getInstance() {
    static Random instance;
    return instance;
}

int Random::generateInt(const int min, const int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_generator);
}

double Random::generateDouble(const double min, const double max) {
    // Use a uniform distribution for floating-point numbers.
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(m_generator);
}