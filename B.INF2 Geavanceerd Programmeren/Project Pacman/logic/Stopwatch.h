/**
 * @file Stopwatch.h
 * @brief Time management and frame rate regulation utility.
 */

#ifndef PACMAN_STOPWATCH_H
#define PACMAN_STOPWATCH_H

#include <chrono>
#include <thread>

/** * @brief Maximum allowable time step (approx. 0.0166s for 60 FPS).
 * Prevents "spiral of death" or huge physics jumps if the game stutters.
 */
constexpr float MAX_DELTA_TIME = 1.0 / 60.0;

/**
 * @class Stopwatch
 * @brief Singleton responsible for tracking elapsed time and capping the frame rate.
 * * The Stopwatch ensures that game movement is frame-rate independent by calculating
 * the time difference between the current and previous frame.
 */
class Stopwatch {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;

    TimePoint m_lastTickTime; /**< The timestamp of the previous tick. */
    double m_deltaTime;       /**< Time elapsed since the last frame in seconds. */

    /** @brief Private constructor initializes the first tick time. */
    Stopwatch();

public:
    // --- Singleton Pattern Enforcement ---
    Stopwatch(const Stopwatch&) = delete;
    Stopwatch& operator=(const Stopwatch&) = delete;

    /**
     * @brief Accesses the global Stopwatch instance.
     * @return Reference to the single Stopwatch instance.
     */
    static Stopwatch& getInstance();

    /**
     * @brief Updates the delta time and enforces a frame rate cap.
     * * Calculates how long the last frame took. If it was faster than
     * MAX_DELTA_TIME, it may put the thread to sleep to maintain
     * a consistent 60 FPS.
     */
    void tick();

    /**
     * @brief Retrieves the time elapsed during the last frame.
     * @return The current delta time in seconds (e.g., 0.016).
     */
    [[nodiscard]] double getDeltaTime() const;
};

#endif // PACMAN_STOPWATCH_H