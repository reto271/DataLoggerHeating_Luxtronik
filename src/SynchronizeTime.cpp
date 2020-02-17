#include <iostream>
#include <ctime>
#include <unistd.h>

#include "SynchronizeTime.hpp"

SynchronizeTime::SynchronizeTime()
{
}

std::time_t SynchronizeTime::waitForMinute()
{
    struct tm* currentLocalTime;

    std::time_t currentUnixTime = std::time(nullptr);
    currentLocalTime = std::localtime(&currentUnixTime);

    while (0 != currentLocalTime->tm_sec) {
        sleep(1);
        currentUnixTime = std::time(nullptr);
        currentLocalTime = std::localtime(&currentUnixTime);
    }
    return currentUnixTime;
}

bool SynchronizeTime::isFullHour()
{
    struct tm* currentLocalTime;

    std::time_t currentUnixTime = std::time(nullptr);
    currentLocalTime = std::localtime(&currentUnixTime);

    return ((0 == currentLocalTime->tm_sec) && (0 == currentLocalTime->tm_min));
}
