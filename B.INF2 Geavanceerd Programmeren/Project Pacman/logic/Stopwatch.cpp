#include "Stopwatch.h"

Stopwatch::Stopwatch() : m_lastTickTime(Clock::now()), m_deltaTime(0.0) {}

Stopwatch& Stopwatch::getInstance() {
    static Stopwatch instance;
    return instance;
}

void Stopwatch::tick() {
    TimePoint currentTime = Clock::now();

    std::chrono::duration<double> duration = currentTime - m_lastTickTime;

    if (const double rawDelta = duration.count(); rawDelta < MAX_DELTA_TIME) {
        const auto required_wait = std::chrono::duration<double>(MAX_DELTA_TIME - rawDelta);

        std::this_thread::sleep_for(required_wait);

        currentTime = Clock::now();
        duration = currentTime - m_lastTickTime;
        m_deltaTime = duration.count();

    } else {
        m_deltaTime = rawDelta;
    }

    m_lastTickTime = currentTime;
}

double Stopwatch::getDeltaTime() const { return m_deltaTime; }