/**
 * @file Random.h
 * @brief Singleton utility for consistent pseudorandom number generation.
 */

#ifndef PACMAN_RANDOM_H
#define PACMAN_RANDOM_H

#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

/**
 * @class Random
 * @brief A Singleton class providing high-quality random numbers and collection sampling.
 * * This class wraps the C++ Standard Library's Mersenne Twister engine to ensure
 * that the random state is maintained throughout the application's lifecycle.
 */
class Random {
    std::mt19937 m_generator; /**< The Mersenne Twister engine. */

    /** @brief Private constructor to enforce Singleton pattern. */
    Random();

public:
    // Delete copy constructor and assignment operator to prevent duplicates.
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    /**
     * @brief Accesses the global Random instance.
     * @return A reference to the static Random instance.
     */
    static Random& getInstance();

    /**
     * @brief Generates a random integer within a closed range [min, max].
     * @param min The lower bound (inclusive).
     * @param max The upper bound (inclusive).
     * @return A random integer between min and max.
     */
    int generateInt(int min, int max);

    /**
     * @brief Generates a random double within a range [min, max).
     * @param min The lower bound (inclusive).
     * @param max The upper bound (exclusive).
     * @return A random double-precision value.
     */
    double generateDouble(double min, double max);

    /**
     * @brief Selects a random element from a std::vector.
     * @tparam T The type of elements in the vector.
     * @param vec The vector to sample from.
     * @return A const reference to a randomly selected element.
     * @throws std::logic_error if the vector is empty.
     */
    template <typename T>
    const T& getRandomElement(const std::vector<T>& vec) {
        if (vec.empty()) {
            throw std::logic_error("Cannot get a random element from an empty vector.");
        }

        int randomIndex = generateInt(0, static_cast<int>(vec.size() - 1));
        return vec[randomIndex];
    }

    /**
     * @brief Selects a random element from a std::set.
     * @note This operation is O(N) due to iterator advancement.
     * @tparam T The type of elements in the set.
     * @param s The set to sample from.
     * @return A const reference to a randomly selected element.
     * @throws std::logic_error if the set is empty.
     */
    template <typename T>
    const T& getRandomElement(const std::set<T>& s) {
        if (s.empty()) {
            throw std::logic_error("Cannot get a random element from an empty set.");
        }

        int randomIndex = generateInt(0, static_cast<int>(s.size() - 1));
        auto it = s.begin();
        std::advance(it, randomIndex);
        return *it;
    }
};

#endif // PACMAN_RANDOM_H