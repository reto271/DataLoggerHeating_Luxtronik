#pragma once

#include <iostream>
#include <cstring>

/// Stores the project version and provides the version for all sub-projects. The release process is as follows.
/// 1) Assume the current released version 1.02
/// 2) Upon the next change the version is set to 1.03B01
/// 3) The next change will get version 1.03B02
/// 4) ...
/// 5) As soon as the Bxx is stable and working properly the Bxx is removed and set to "" empty string. -> 1.03.
class ProgramVersion
{
public:
    /// Constructs the version handler
    ProgramVersion();

    /// Prints the program version along with the provided program name, e.g. MonitorHeating.
    /// \param[in] programName is the name of the program (sub-project), e.g. DataDecoder, ...
    void printProgramInfo(std::string programName);

private:
    std::string SoftwareVersion = "V01.03";   ///< Major and minor SW version
    std::string MinorVersion = "";         ///< Format "B01" or empty string for release
};
