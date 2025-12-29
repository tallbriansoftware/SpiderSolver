#pragma once

#include "Utils/ChronoTimer.h"

class CommandLineArguments;


class GameTimeLimiter
{
public:
    GameTimeLimiter(const CommandLineArguments& args);
    bool TimeIsUp();

private:
    int m_limitSeconds;
    ChronoTimer m_timer;
    int64_t m_startMilliSeconds;
};