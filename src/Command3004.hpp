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


class DecodeValueResponse
{
public:
    DecodeValueResponse();

    void setRecieveBuffer(RecDataStoragePtr bufferPtr);

    uint32_t getNumberOfEntries() const;

    void decode();
    char serialize();

private:
    RecDataStoragePtr m_bufferPtr;
};
