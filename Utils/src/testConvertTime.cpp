#include <ctime>
#include <iostream>
#include <stdexcept>      // std::invalid_argument
#include <string>         // std::string

void printTime(std::time_t timeValue, std::string title);
bool convertValue(std::string timeString, std::time_t& timeValue);

int main(int argc, char* argv[])
{
    std::cout << "--------------------" << std::endl;

    std::time_t cmdLineTimeValue;
    std::time_t currentUnixTime = std::time(nullptr);
    std::time_t maxTimeWith32bits = 0xffffffff;

    printTime(currentUnixTime, "Current Time");
    printTime(maxTimeWith32bits, "Max time using 32bit");

    if(argc == 2) {
        std::string cmdLineArg = argv[1];
        if(true == convertValue(cmdLineArg, cmdLineTimeValue)) {
            printTime(cmdLineTimeValue, "Command Line Value");
        } else {
            std::cout << "Could not convert value: '" << cmdLineArg << "'" << std::endl;
        }
    }
}

void printTime(std::time_t timeValue, std::string title)
{
    std::cout << "Title: '" << title << "'" << std::endl;
    std::cout << timeValue << " / 0x" << std::hex << timeValue << std::dec << " seconds since the Epoch. Time: " << std::asctime(std::localtime(&timeValue));
    std::cout << "--------------------" << std::endl;
}

bool convertValue(std::string timeString, std::time_t& timeValue)
{
    std::time_t value;
    size_t nrCharsProcessed = timeString.length();

    timeValue = 0;

    // Is it a hex number?
    size_t posInStr = timeString.find("0x", 0);
    if((std::string::npos != posInStr) && (0 == posInStr)) {
        std::time_t value;
        // it is possibly a hex value
        try {
            value = std::stoul(timeString.c_str(), &nrCharsProcessed, 16);
            timeValue = value;
            return true;
        } catch(const std::invalid_argument& ia) {
            std::cout << "ERROR1" << std::endl;
            std::cerr << "Could not convert to hex number: " << ia.what() << std::endl;
            return false;
        }
    }

    // Try plain number
    try {
        value = std::stoul(timeString.c_str(), &nrCharsProcessed, 10);
        timeValue = value;
        return true;
    } catch(const std::invalid_argument& ia) {
        std::cout << "ERROR2" << std::endl;
        std::cerr << "Could not convert to hex number: " << ia.what() << std::endl;
        return false;
    }
    return false;
}
