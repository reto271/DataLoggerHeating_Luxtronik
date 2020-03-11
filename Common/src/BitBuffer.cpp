#include "BitBuffer.hpp"
#include <iostream>

BitBuffer::BitBuffer()
    : m_nrBitsInBuffer(0)
    , m_writePos(0)
    , m_readBitPos(0)
{
}

BitBuffer::~BitBuffer()
{
}

void BitBuffer::appendBits(uint32_t value, uint8_t nrBits)
{
    uint32_t nrBitsInVector = 8 * m_buffer.size();
    int32_t nrAdditionalReqBits = (m_nrBitsInBuffer + nrBits) - nrBitsInVector;
    if (0 < nrAdditionalReqBits) {
        // need more buffer space
        uint8_t nrReqBytes = (nrAdditionalReqBits-1 / 8) + 1;
        m_buffer.resize(m_buffer.size() + nrReqBytes);
    }
    for (uint8_t cnt = 0; cnt < nrBits; cnt++) {
        appendBit(0x1 & value);
        value = value >> 1;
    }
}

void BitBuffer::restartReading()
{
    m_readBitPos = 0;
}

uint32_t BitBuffer::getBits(uint8_t nrBits)
{
    uint32_t response = 0;
    for (uint32_t cnt = 0; cnt < nrBits; cnt++) {
        uint32_t bit = getBit() << cnt;
        response += bit;
    }
    return response;
}

uint8_t* BitBuffer::getReferenceToBuffer(uint32_t& nrBitsInBuffer)
{
    nrBitsInBuffer = m_nrBitsInBuffer;
    return m_buffer.data();
}

void BitBuffer::appendBit(uint32_t value)
{
    if (0 != value) {
        uint32_t bytePos = m_writePos >> 3;
        uint8_t bitNr = m_writePos & 0x07;
        uint8_t bitPos = 0x1 << bitNr;

        std::cout << "appendBit: bytePos: " << bytePos <<
            ", bitNr: " << static_cast<uint16_t>(bitNr) <<
            ", bitPos: " << static_cast<uint16_t>(bitPos) << std::endl;

        m_buffer[bytePos] += bitPos;
    } else {
        uint32_t bytePos = m_writePos >> 3;
        uint8_t bitNr = m_writePos & 0x07;
        uint8_t bitPos = 0x1 << bitNr;

        std::cout << "appendBit: bytePos: " << bytePos <<
            ", bitNr: " << static_cast<uint16_t>(bitNr) <<
            ", bitPos: " << static_cast<uint16_t>(bitPos) <<
            " - don't set it" << std::endl;
    }
    m_writePos++;
    m_nrBitsInBuffer++;
}

uint32_t BitBuffer::getBit()
{
    uint32_t bytePos = m_readBitPos >> 3;
    uint8_t bitNr = m_readBitPos & 0x07;
    uint8_t bitPos = 0x1 << bitNr;

    std::cout << "getBit: bytePos: " << bytePos <<
        ", bitNr: " << static_cast<uint16_t>(bitNr) <<
        ", bitPos: " << static_cast<uint16_t>(bitPos);

    m_readBitPos++;
    if (0 == (m_buffer[bytePos] & bitPos)) {
        std::cout << "    : 0" << std::endl;
        return 0;
    } else {
        std::cout << "    : 1" << std::endl;
        return 1;
    }
}

void BitBuffer::printContent()
{
    std::cout << "  m_nrBitsInBuffer  : " << m_nrBitsInBuffer << std::endl;
    std::cout << "  m_writePos        : " << m_writePos << std::endl;
    std::cout << "  m_readBitPos      : " << m_readBitPos << std::endl;

    uint32_t nrBytes = m_nrBitsInBuffer >> 3;

    for(uint32_t cnt = 0; cnt < nrBytes; cnt++) {
        std::cout << std::hex << ", 0x" << static_cast<uint16_t>(m_buffer[cnt]);
    }
    std::cout << std::endl;
}
