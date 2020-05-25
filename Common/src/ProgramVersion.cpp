#include "ProgramVersion.hpp"

ProgramVersion::ProgramVersion()
{
}

ProgramVersion::~ProgramVersion()
{
}

void ProgramVersion::printProgramInfo(std::string programName)
{
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << " Program: '" << programName << "'" << std::endl;
    std::cout << " Version: " << SoftwareVersion << std::endl;
    if(true == MinorVersion.empty()) {
        std::cout << " Status: released" << std::endl;
    } else {
        std::cout << " Minor Version: " << MinorVersion << std::endl;
    }
    std::cout << "-----------------------------------------------------" << std::endl;
}
