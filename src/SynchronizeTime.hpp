#pragma once

/// Synchronize the program with the clock of the system.
class SynchronizeTime
{
public:
    SynchronizeTime();

    /// Used to synchronize with the full minute, returns true if the second = 0. The function is
    /// either returning true/false if is non-blocking or waits until the second = 0.
    /// \param[in] blockingWait: if true, the function waits until the full minute. Otherwise it returns true/false.
    /// \return true if the time in sec = 0
    bool waitForMinute(bool blockingWait);

    /// Returns true if the full hour is reached, namely second = 0 & minute = 0.
    /// \return true if the full hour is reached, false otherwise.
    bool isFullHour();
private:
};
