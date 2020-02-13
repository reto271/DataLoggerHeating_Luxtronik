#pragma once

#include <stdint.h>
#include <string>

#include "ReceiveDataBuffer.hpp"


// Command Values 3004

typedef struct
{
    uint32_t cmdId;
    // identifier is not used
    std::string description;
    uint32_t converstionDivisor;
    std::string unit;
} ValueEntry;


class DecodeValueResponse
{
public:
    DecodeValueResponse();

    void setRecieveBuffer(RecDataStoragePtr bufferPtr);

    uint32_t getNumberOfEntries();

    void decode();

private:
    RecDataStoragePtr m_bufferPtr;
};
