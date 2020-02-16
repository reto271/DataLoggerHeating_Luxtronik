#include <iostream>
#include <ctime>
#include <unistd.h>

#include "SynchronizeTime.hpp"

SynchronizeTime::SynchronizeTime()
{
}

bool SynchronizeTime:: waitForMinute(bool blockingWait)
{
    struct tm* currentLocalTime;

    std::time_t currentUnixTime = std::time(nullptr);
    currentLocalTime = std::localtime(&currentUnixTime);

    if (true == blockingWait) {
        while (0 != currentLocalTime->tm_sec) {
            sleep(1);
            currentUnixTime = std::time(nullptr);
            currentLocalTime = std::localtime(&currentUnixTime);
        }
        return true;
    } else {
        return (0 == currentLocalTime->tm_sec);
    }
}

bool SynchronizeTime::isFullHour()
{
    struct tm* currentLocalTime;

    std::time_t currentUnixTime = std::time(nullptr);
    currentLocalTime = std::localtime(&currentUnixTime);

    return ((0 == currentLocalTime->tm_sec) && (0 == currentLocalTime->tm_min));
}
