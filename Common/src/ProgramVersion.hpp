#pragma once

#include <iostream>
#include <cstring>

class ProgramVersion
{
public:
    ProgramVersion();
    ~ProgramVersion();

    void printProgramInfo(std::string programName);

private:
    std::string SoftwareVersion = "V01.01";
    std::string MinorVersion = "";         // format "B01" or empty for release
};
