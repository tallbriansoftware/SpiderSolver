#include "GameTimeLimiter.h"

#include "CommandLineArguments.h"
#include "Utils/ChronoTimer.h"

#include <ctime>


GameTimeLimiter::GameTimeLimiter(const CommandLineArguments& args)
    : m_limitSeconds(0)
{
    m_limitSeconds = args.GetLimitSeconds();
    if (m_limitSeconds != 0)
    {
        m_timer.Start();
    }
}

bool GameTimeLimiter::TimeIsUp()
{
    if (m_limitSeconds == 0)
        return false;

    int64_t now = m_timer.ReadMilliseconds();
    
    return (now / 1000) > m_limitSeconds;
}