#pragma once

#include <stdint.h>
#include <vector>

/// Compact buffer to store data. For each value to be stored the user must provide the number of bits the
///  values requires. The buffer stores only these bits. To minimize the buffer, only these bits are stored
///  the next variable is appended consecutively. The buffer allocates byte-by-byte on demand. At end there are
///  at most 7 unused bits.
class BitBuffer
{
public:
    /// Constructs an empty BitBuffer.
    BitBuffer();

    /// Constructs a BitBuffer, using the vector to initialize the buffer.
    /// \param[in] data is a vector of 8-bit data. It is used to initialize the buffer.
    BitBuffer(std::vector<uint8_t> data);

    /// Destructor of the buffer, frees the allocated memory.
    ~BitBuffer();

    /// Appends an unsigned value to the buffer, at most 32bit at once. At least 1 bit must be written.
    /// \param[in] value to be added to the buffer.
    /// \param[in] nrBits to be stored in the buffer. The right most bits are used.
    void appendValue(uint32_t value, uint8_t nrBits);

    /// Appends a signed value to the buffer.
    /// \param[in] value to be added to the buffer.
    /// \param[in] nrBits to be stored in the buffer. Add 1 bit for the sign. Minimum number of bits is 2, maximum is 32.
    void appendValue(int32_t value, uint8_t nrBits);

    /// Restarts reading the buffer at the beginning. (Set read pointer to the first bit in the buffer).
    void restartReading();

    /// Reads an unsigned value from the buffer.
    /// \param[out] value returned from the buffer
    /// \param[in] nrBits to be read from the buffer. At least 1, at most 32.
    void getValue(uint32_t& value, uint8_t nrBits);

    /// Reads a signed value from the buffer.
    /// \param[out] value returned from the buffer
    /// \param[in] nrBits to be read from the buffer. At least 2, at most 32. Remark: Include the sign.
    void getValue(int32_t& value, uint8_t nrBits);

    /// Gets a reference to the internal buffer. Use the function with care!
    /// \param[out] nrBytesInBuffer is the number of bytes currently in the buffer. Remember: the last byte might
    ///   not be fully used. The unused is not initialized.
    /// \return reference to the buffer
    uint8_t* getReferenceToBuffer(uint32_t& nrBytesInBuffer);

    /// Returns the number of used bits. Is not necessary the number of bytes times 8, since the last byte might
    ///  not be fully used.
    /// \return Number of used bits.
    uint32_t getNumberBitsInBuffer();

    /// Prints the content of the buffer to the STDOUT.
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

    /// Appends the provided bits to the buffer. The function is used by the public appendValue
    ///  functions. These functions take care about signed/unsigned issues. This function just
    ///  stores provided the bit pattern.
    /// \param[in] value to be placed in the BitBuffer
    /// \param[in] nrBits the number of bits to be added to the buffer.
    void appendValuesToBuffer(uint32_t value, uint8_t nrBits);

    /// Appends a single bit to the BitBuffer
    /// \param[in] value is the bit to be added. The least significant bit is considered.
    void appendBit(uint32_t value);

    /// Get a single bit from the buffer.
    /// \return the bit is returned in the least significant bit position.
    uint32_t getBit();


    std::vector<uint8_t> m_buffer;    ///< Vector containing the bit data
    uint32_t m_nrBitsInBuffer;        ///< Number of bits stored in the buffer
    uint32_t m_writePos;              // to be eliminated
    uint32_t m_readBitPos;            ///< Read position while reading. Points to the next bit to be read.
};
