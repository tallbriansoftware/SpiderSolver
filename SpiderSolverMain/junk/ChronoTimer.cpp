#include "Chronotimer.h"


ChronoTimer::ChronoTimer()
{
    m_start = m_end = std::chrono::system_clock::now();
    m_running = true;
}


void ChronoTimer::Start()
{
    m_start = m_end = std::chrono::system_clock::now();
    m_running = true;
}


void ChronoTimer::Stop()
{
    m_end = std::chrono::system_clock::now();
    m_running = false;
}


void ChronoTimer::Reset()
{
    m_start = m_end = std::chrono::system_clock::now();
    m_running = false;
}


void ChronoTimer::Restart()
{
    m_start = m_end = std::chrono::system_clock::now();
    m_running = true;
}


int64_t ChronoTimer::ReadMicroseconds()
{
    return ReadNanoseconds() / 1000;
}


int64_t ChronoTimer::ReadNanoseconds()
{
    auto end = (m_running) ? std::chrono::system_clock::now() : m_end;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_start).count();
}
