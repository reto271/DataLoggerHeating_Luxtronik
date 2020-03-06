#pragma once

#include <ctime>
#include <stdint.h>

/// Synchronize the program with the clock of the system.
class SynchronizeTime
{
public:
    SynchronizeTime();

    /// Used to synchronize with the full minute, returns true if the second = 0.
    /// \return Unix time in seconds since 1. Jan 1970.
    std::time_t waitForMinute();

    /// Returns true if the full hour is reached, namely second = 0 & minute = 0.
    /// \return true if the full hour is reached, false otherwise.
    bool isFullHour(bool upateTime);

    /// Returns the hour
    /// \return hour in the range 0..23
    uint32_t getHour(bool upateTime);

private:
    std::time_t m_currentUnixTime;
};
