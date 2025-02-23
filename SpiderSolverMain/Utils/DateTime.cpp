#include "DateTime.h"

#include <chrono>

std::string GetCurrentDate()
{
    auto now = std::chrono::system_clock::now();
    auto rawTime = std::chrono::system_clock::to_time_t(now);
    struct tm timeInfo;
    char buffer[80];

    localtime_s(&timeInfo, &rawTime);
    strftime(buffer, 80, "%Y-%m-%d", &timeInfo);

    std::string retVal(buffer);
    return retVal;
}
