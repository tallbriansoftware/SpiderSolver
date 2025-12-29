#pragma once

#include <chrono>

class ChronoTimer
{
public:
    ChronoTimer();
    void Start();
    void Stop();
    void Reset();
    void Restart();
    int64_t ReadNanoseconds();
    int64_t ReadMicroseconds();
    int64_t ReadMilliseconds();

private:
    std::chrono::time_point<std::chrono::system_clock> m_start;
    std::chrono::time_point<std::chrono::system_clock> m_end;
    bool m_running;
};