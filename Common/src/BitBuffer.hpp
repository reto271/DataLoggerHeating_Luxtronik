#pragma once

#include <stdint.h>
#include <vector>

class BitBuffer
{
public:
    BitBuffer();
    ~BitBuffer();

    void appendValue(uint32_t value, uint8_t nrBits);
    void appendValue(int32_t value, uint8_t nrBits);

    void restartReading();
    void getValue(uint32_t& value, uint8_t nrBits);
    void getValue(int32_t& value, uint8_t nrBits);

    uint8_t* getReferenceToBuffer(uint32_t& nrBitsInBuffer);

    void printContent();
private:

    void appendBit(uint32_t value);
    uint32_t getBit();

    std::vector<uint8_t> m_buffer;
    uint32_t m_nrBitsInBuffer;
    uint32_t m_writePos;
    uint32_t m_readBitPos;
};
