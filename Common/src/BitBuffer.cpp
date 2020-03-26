#include "BitBuffer.hpp"
#include <iostream>
#include <assert.h>

BitBuffer::BitBuffer()
    : m_nrBitsInBuffer(0)
    , m_writePos(0)
    , m_readBitPos(0)
{
}

BitBuffer::~BitBuffer()
{
}

void BitBuffer::appendValue(uint32_t value, uint8_t nrBits)
{
    // Make sure the value fits
    uint32_t bitMask = getUnsignedBitMask(nrBits);
    if(value != (value & bitMask)) {
        std::cout << "value: " << value << " / 0x" << std::hex << value << std::dec << ", nrBits: " << static_cast<uint32_t>(nrBits) << std::endl;
        assert(value == (value & bitMask));
    }

    // Finally write values to buffer
    appendValuesToBuffer(value, nrBits);
}

void BitBuffer::appendValue(int32_t value, uint8_t nrBits)
{
    uint32_t u32value = *reinterpret_cast<uint32_t*>(&value);
    uint32_t bitMask = getSignedBitMask(nrBits);

    // Range check for signed values, make sure there is enough space for value and sign (still stored in two's complement)
    if(0 == (0x80000000 & value)) {
        if(0 != (~bitMask & value)) {
            // Positive values, all bits outside the mask must be 0
            std::cout << "value: 0x" << std::hex << value << ", bitMaskForValue: 0x" << bitMask << std::dec << ", nrBits: " << static_cast<uint32_t>(nrBits) << std::endl;
            assert(0 == (~bitMask & value));
        }
    } else {
        // Negative value, all bits outside the mask must be 1
        if(~bitMask != (~bitMask & value)) {
            std::cout << "value: 0x" << std::hex << value << ", bitMaskForValue: 0x" << bitMask << std::dec << ", nrBits: " << static_cast<uint32_t>(nrBits) << std::endl;
            assert(~bitMask == (~bitMask & value));
        }
    }

    // Finally write values to buffer
    appendValuesToBuffer(u32value, nrBits);
}

void BitBuffer::restartReading()
{
    m_readBitPos = 0;
}

void BitBuffer::getValue(uint32_t& value, uint8_t nrBits)
{
    value = 0;
    for(uint32_t cnt = 0; cnt < nrBits; cnt++) {
        uint32_t bit = getBit() << cnt;
        value += bit;
    }
}

void BitBuffer::getValue(int32_t& value, uint8_t nrBits)
{
    uint32_t currentBit;
    value = 0;

    for(int32_t cnt = 0; cnt < 32; cnt++) {
        if(cnt < nrBits) {
            currentBit = getBit() << cnt;
            value += currentBit;
        } else {
            currentBit <<= 1;
            value += currentBit;
        }
    }
}

uint8_t* BitBuffer::getReferenceToBuffer(uint32_t& nrBitsInBuffer)
{
    nrBitsInBuffer = m_nrBitsInBuffer;
    return m_buffer.data();
}

void BitBuffer::printContent()
{
    std::cout << "--- printContent ---------" << std::endl;
    std::cout << "  m_nrBitsInBuffer  : " << m_nrBitsInBuffer << std::endl;
    std::cout << "  m_writePos        : " << m_writePos << std::endl;
    std::cout << "  m_readBitPos      : " << m_readBitPos << std::endl;

    uint32_t nrBytes = (m_nrBitsInBuffer >> 3) + 1;

    for(uint32_t cnt = 0; cnt < nrBytes; cnt++) {
        std::cout << std::hex << ", 0x" << static_cast<uint16_t>(m_buffer[cnt]);
    }
    std::cout << std::dec << std::endl;
    std::cout << "--- printContent ---------" << std::endl;
}

uint32_t BitBuffer::getUnsignedBitMask(uint8_t nrBits)
{
    assert(0 < nrBits);
    return static_cast<uint32_t>(((static_cast<uint64_t>(0x1) << nrBits) - 1));
}

uint32_t BitBuffer::getSignedBitMask(uint8_t nrBits)
{
    assert(1 < nrBits);  // one bit sign, one bit value
    return ((0x1 << (nrBits - 1)) - 1);
}

void BitBuffer::appendValuesToBuffer(uint32_t value, uint8_t nrBits)
{
    uint32_t nrBitsInVector = 8 * m_buffer.size();
    int32_t nrAdditionalReqBits = (m_nrBitsInBuffer + nrBits) - nrBitsInVector;
    if(0 < nrAdditionalReqBits) {
        // need more buffer space
        uint8_t nrReqBytes = (nrAdditionalReqBits - 1 / 8) + 1;
        m_buffer.resize(m_buffer.size() + nrReqBytes);
    }
    for(uint8_t cnt = 0; cnt < nrBits; cnt++) {
        appendBit(0x1 & value);
        value = value >> 1;
    }
}

void BitBuffer::appendBit(uint32_t value)
{
    uint32_t bytePos = m_writePos >> 3;
    uint8_t bitNr = m_writePos & 0x07;
    uint8_t bitPos = 0x1 << bitNr;

    if(0 != value) {
        m_buffer[bytePos] += bitPos;
#if defined(DEBUG_OUTPUT_BIT_BUFFER)
        std::cout << "appendBit: bytePos: " << bytePos <<
            ", bitNr: " << static_cast<uint16_t>(bitNr) <<
            ", bitPos: " << static_cast<uint16_t>(bitPos) << std::endl;
    } else {
        std::cout << "appendBit: bytePos: " << bytePos <<
            ", bitNr: " << static_cast<uint16_t>(bitNr) <<
            ", bitPos: " << static_cast<uint16_t>(bitPos) <<
            " - don't set it" << std::endl;
#endif // DEBUG_OUTPUT_BIT_BUFFER
    }
    m_writePos++;
    m_nrBitsInBuffer++;
}

uint32_t BitBuffer::getBit()
{
    uint32_t bytePos = m_readBitPos >> 3;
    uint8_t bitNr = m_readBitPos & 0x07;
    uint8_t bitPos = 0x1 << bitNr;

#if defined(DEBUG_OUTPUT_BIT_BUFFER)
    std::cout << "getBit: bytePos: " << bytePos <<
        ", bitNr: " << static_cast<uint16_t>(bitNr) <<
        ", bitPos: " << static_cast<uint16_t>(bitPos);
#endif // DEBUG_OUTPUT_BIT_BUFFER


    assert(m_readBitPos < m_nrBitsInBuffer);
    if(m_readBitPos >= m_nrBitsInBuffer) {
        std::cout << "m_readBitPos: " << m_readBitPos << ", m_nrBitsInBuffer: " << m_nrBitsInBuffer << std::endl;
        assert(m_readBitPos < m_nrBitsInBuffer);
    }
    m_readBitPos++;
    if(0 == (m_buffer[bytePos] & bitPos)) {
#if defined(DEBUG_OUTPUT_BIT_BUFFER)
        std::cout << "    : 0" << std::endl;
#endif // DEBUG_OUTPUT_BIT_BUFFER
        return 0;
    } else {
#if defined(DEBUG_OUTPUT_BIT_BUFFER)
        std::cout << "    : 1" << std::endl;
#endif // DEBUG_OUTPUT_BIT_BUFFER
        return 1;
    }
}
