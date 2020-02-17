#pragma once

#include <stdint.h>
#include <string>

#include "ReceiveDataBuffer.hpp"


// Command Values 3004

//typedef float (* ConversionFunctionDivisor)(const uint32_t value, const uint32_t divisor);
//typedef bool (* ConversionFunctionBool)(const uint32_t value);


float conversionFunctionDivisor(const uint32_t value, const uint32_t divisor);
bool conversionFunctionBool(const uint32_t value);



//typedef union
//{
//    ConversionFunctionDivisor convFuncDivisor;
//    ConversionFunctionBool convFuncBool;
//} ConversionFunctions;

typedef struct
{
    uint32_t cmdId;
    // identifier is not used
    std::string description;
    uint32_t conversionDivisor;
    std::string unit;
} ValueEntry;


class ValueResponse
{
public:
    ValueResponse(RecDataStoragePtr receiveDataPtr, std::time_t currentUnixTime);

    uint32_t getNumberOfEntries() const;

    void decode();
    char serialize();

private:
    /// Add the current unix time in seconds since January, 1st 1970 to the data file.
    ///  Use 8 bytes independent on the HW/Compiler.
    /// \param[in] wf: Reference to the file stream.
    /// \param[in] unixTime: Unix time to be written to the file.
    void addUnixTimeToBuffer(std::ofstream& wf, std::time_t unixTime);

    std::string fileNameFromDate();


    RecDataStoragePtr m_responsePtr;
    std::time_t m_currentUnixTime;
};
