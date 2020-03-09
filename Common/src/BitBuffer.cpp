#include "BitBuffer.hpp"


BitBuffer::BitBuffer()
    : m_nrBitsInBuffer(0)
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
}

uint8_t* BitBuffer::getReferenceToBuffer(uint32_t& nrBitsInBuffer)
{
    nrBitsInBuffer = m_nrBitsInBuffer;
    return m_buffer.data();
}

void BitBuffer::appendBit(uint32_t value)
{
    m_buffer[2] = 0x11;
}

uint32_t BitBuffer::getBit()
{
    return 0;
}
