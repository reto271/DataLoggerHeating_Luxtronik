#pragma once

#include <stdint.h>
#include <vector>

class BitBuffer
{
public:
    BitBuffer();
    BitBuffer(std::vector<uint8_t> data);
    ~BitBuffer();

    void appendValue(uint32_t value, uint8_t nrBits);
    void appendValue(int32_t value, uint8_t nrBits);

    void restartReading();
    void getValue(uint32_t& value, uint8_t nrBits);
    void getValue(int32_t& value, uint8_t nrBits);

    uint8_t* getReferenceToBuffer(uint32_t& nrBytesInBuffer);
    uint32_t getNumberBitsInBuffer();

    void printContent();

protected:
    /// Returns the bit mask for the unsigned integers.
    /// \param[in] nrBits: Number of bits used in buffer
    /// \return bit mask for the value
    uint32_t getUnsignedBitMask(uint8_t nrBits);

    /// Returns the bit mask for the signed integers, WITHOUT the sign.
    /// \param[in] nrBits: Number of bits used in buffer INCLUSIVE sign.
    /// \return bit mask for the value, excluding the sign
    uint32_t getSignedBitMask(uint8_t nrBits);

private:
    void appendValuesToBuffer(uint32_t value, uint8_t nrBits);
    void appendBit(uint32_t value);
    uint32_t getBit();

    std::vector<uint8_t> m_buffer;
    uint32_t m_nrBitsInBuffer;
    uint32_t m_writePos;
    uint32_t m_readBitPos;
};
