#include "SynchronizeTime.hpp"

#include <unistd.h>


SynchronizeTime::SynchronizeTime()
{
}

std::time_t SynchronizeTime::waitForMinute()
{
    struct tm* currentLocalTime;

    m_currentUnixTime = std::time(nullptr);
    currentLocalTime = std::localtime(&m_currentUnixTime);

    while(0 != currentLocalTime->tm_sec) {
        sleep(1);
        m_currentUnixTime = std::time(nullptr);
        currentLocalTime = std::localtime(&m_currentUnixTime);
    }
    return m_currentUnixTime;
}

bool SynchronizeTime::isFullHour(bool updateTime)
{
    struct tm* currentLocalTime;

    if(true == updateTime) {
        m_currentUnixTime = std::time(nullptr);
    }
    currentLocalTime = std::localtime(&m_currentUnixTime);

    return ((0 == currentLocalTime->tm_sec) && (0 == currentLocalTime->tm_min));
}

uint32_t SynchronizeTime::getHour(bool updateTime)
{
    struct tm* currentLocalTime;

    if(true == updateTime) {
        m_currentUnixTime = std::time(nullptr);
    }
    currentLocalTime = std::localtime(&m_currentUnixTime);

    return currentLocalTime->tm_hour;
}
