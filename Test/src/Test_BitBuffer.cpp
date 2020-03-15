#include <gtest/gtest.h>
#include <stdint.h>

#include "Common/src/BitBuffer.hpp"

namespace
{
// using ::testing::StrictMock;
// using ::testing::Return;
// using ::testing::Exactly;
// using ::testing::_;
// using ::testing::HasSubstr;

class BitBufferTest : public testing::Test
{
protected:
    void SetUp() override
    {
        srand(time(NULL));
    }

    void TearDown() override
    {
        //Dump Trace buffer
    }

    void getRandomValue(uint32_t& value, uint16_t& bitWidth)
    {
        // rand returns numbers 0 and RAND_MAX. -> it returns only 31 bit
        assert(RAND_MAX == 0x7fffffff); //
        bitWidth = (rand() % 32) + 1;
        value = (rand() << 1) ^ rand();   // exor two rand variables, shift one
        EXPECT_TRUE(0 < bitWidth);
        EXPECT_TRUE(32 >= bitWidth);
    }

    void getRandomValue(int32_t& value, uint16_t& bitWidth)
    {
        uint32_t u32value;
        getRandomValue(u32value, bitWidth);
        value = *reinterpret_cast<int32_t*>(&u32value);
    }

    int32_t limitValue(int32_t value, uint32_t width)
    {
        assert(0 < width);

        // Calculate masks
        uint32_t bitMask = 0x0;  // remember 1 bit is the sign, e.g. 0xf -> [-8 ... 7]
        uint32_t mask = 0x1;
        uint32_t highestBit;
        for (uint32_t cnt = 0; cnt < width; cnt++){
            bitMask |= mask;
            highestBit = mask;
            mask <<= 1;
        }

        // Limit the value
        int32_t limitedVal;
        limitedVal = value & bitMask;
        highestBit = limitedVal & highestBit;

        // Add leading sign bits
        for (int32_t bitPos = 4; bitPos < 32; bitPos++){
            highestBit <<= 1;
            limitedVal |= highestBit;
        }
//        std::cout << "counter: " << i << " / 0x" << std::hex << i << std::dec <<
//            ", limited: " << limitedVal << " / 0x" << std::hex << limitedVal << std::dec << std::endl;
        return limitedVal;
    }

    //StrictMock<test::Mock_DummyDriver> m_dummyDriver;
    BitBuffer m_testBuffer;
};

TEST_F(BitBufferTest, clearAll32bits)
{
    uint32_t value;

    m_testBuffer.appendValue(static_cast<uint32_t>(0), 32);

    m_testBuffer.restartReading();
    m_testBuffer.getValue(value, 32);

    EXPECT_EQ(0, value);
}

TEST_F(BitBufferTest, setAll32bits)
{
    uint32_t value;
    m_testBuffer.appendValue(static_cast<uint32_t>(0xffffffff), 32);

    m_testBuffer.restartReading();
    m_testBuffer.getValue(value, 32);

    EXPECT_EQ(0xffffffff, value);
}

TEST_F(BitBufferTest, setGet32BitsAtOnce)
{
    uint32_t value;
    uint32_t value4bits;

    m_testBuffer.appendValue(static_cast<uint32_t>(0x12345678), 32);

    m_testBuffer.restartReading();
    m_testBuffer.getValue(value, 32);
    EXPECT_EQ(0x12345678, value);

    m_testBuffer.restartReading();
    m_testBuffer.getValue(value4bits, 4);
    EXPECT_EQ(value4bits, value & 0xf);

//    m_testBuffer.printContent();
}

TEST_F(BitBufferTest, set32BitsAndCheckBuffer)
{
    m_testBuffer.appendValue(static_cast<uint32_t>(0x12345678), 32);

    uint32_t nrBits;
    uint8_t* pBuffer = m_testBuffer.getReferenceToBuffer(nrBits);

    EXPECT_EQ(32, nrBits);
    EXPECT_EQ(0x78, pBuffer[0]);
    EXPECT_EQ(0x56, pBuffer[1]);
    EXPECT_EQ(0x34, pBuffer[2]);
    EXPECT_EQ(0x12, pBuffer[3]);
}

TEST_F(BitBufferTest, set5ByteValues)
{
    for (uint32_t cnt = 0; cnt < 5; cnt++) {
        m_testBuffer.appendValue(10 + cnt, 8);
    }

    uint32_t nrBits;
    uint8_t* pBuffer = m_testBuffer.getReferenceToBuffer(nrBits);

    EXPECT_EQ(40, nrBits);
    EXPECT_EQ(10, pBuffer[0]);
    EXPECT_EQ(11, pBuffer[1]);
    EXPECT_EQ(12, pBuffer[2]);
    EXPECT_EQ(13, pBuffer[3]);
    EXPECT_EQ(14, pBuffer[4]);
}


TEST_F(BitBufferTest, set34bitValues)
{
    uint32_t value;
    for (uint32_t cnt = 0; cnt < 34; cnt++) {
        m_testBuffer.appendValue(cnt & 0x1, 1);
    }

    uint32_t nrBits;
    uint8_t* pBuffer = m_testBuffer.getReferenceToBuffer(nrBits);

    EXPECT_EQ(34, nrBits);
    EXPECT_EQ(0xaa, pBuffer[0]);
    EXPECT_EQ(0xaa, pBuffer[1]);
    EXPECT_EQ(0xaa, pBuffer[2]);
    EXPECT_EQ(0xaa, pBuffer[3]);
    EXPECT_EQ(0x02, pBuffer[4]);

    m_testBuffer.restartReading();
    for (uint32_t cnt = 0; cnt < 34; cnt++) {
        m_testBuffer.getValue(value, 1);
        EXPECT_EQ(value , (cnt & 0x1));
    }
}

TEST_F(BitBufferTest, randomBufferReadWriteUnsignedValues)
{
    typedef struct{
        uint32_t randVal;
        uint16_t bitWidth;
    } RandomNumber;
    const uint32_t NrValuesInBuffer = 44;
    RandomNumber randomNumberValues[NrValuesInBuffer];
    uint32_t value;

    // Write random numbers to buffer and store them in an array
    for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
        uint16_t width;
        getRandomValue(value, width);
//        std::cout << "Write rand value: " << value << " /  0x" << std::hex << value << std::dec << ", bitWidth: " << width << std::endl;
        m_testBuffer.appendValue(value, width);
        randomNumberValues[cnt].randVal = value;
        randomNumberValues[cnt].bitWidth = width;
    }

    // Read back values and compare with the values in buffer
    m_testBuffer.restartReading();
    for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
        uint16_t bitWidth = randomNumberValues[cnt].bitWidth;
//        std::cout << std::endl << "Read nr Bits: " << bitWidth << std::endl;
        m_testBuffer.getValue(value, bitWidth);

        uint32_t bitMask = (0x1 << bitWidth) - 1;
        if (32 == bitWidth) {
            bitMask = 0xffffffff;
        }

//        std::cout << "bitMask: 0x" << std::hex << bitMask << std::dec << std::endl;
//        std::cout << "Value: " << value << " / 0x" << std::hex << value << std::dec << std::endl;
        EXPECT_EQ((randomNumberValues[cnt].randVal & bitMask), value);

    }
}

TEST_F(BitBufferTest, randomBufferReadWriteSignedValues)
{
    typedef struct{
        int32_t randVal;
        uint16_t bitWidth;
    } RandomNumber;
    const uint32_t NrValuesInBuffer = 4;
    RandomNumber randomNumberValues[NrValuesInBuffer];
    int32_t value;

    // Write random numbers to buffer and store them in an array
    for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
        uint16_t width;
        getRandomValue(value, width);

        std::cout << "Write rand value: " << value << " /  0x" << std::hex << value << std::dec << ", bitWidth: " << width << std::endl;
        m_testBuffer.appendValue(value, width);
        randomNumberValues[cnt].randVal = value;
        randomNumberValues[cnt].bitWidth = width;
    }

    std::cout << "---------------------------------" << std::endl;
    m_testBuffer.printContent();
    std::cout << "---------------------------------" << std::endl;


    // Read back values and compare with the values in buffer
    m_testBuffer.restartReading();
    for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
        uint16_t bitWidth = randomNumberValues[cnt].bitWidth;
        std::cout << std::endl << "Read nr Bits: " << bitWidth << std::endl;
        m_testBuffer.getValue(value, bitWidth);

        int32_t expectedValue = limitValue(randomNumberValues[cnt].randVal, randomNumberValues[cnt].bitWidth);


        std::cout << "Value: " << value << " / 0x" << std::hex << value << std::dec << std::endl;
        EXPECT_EQ(expectedValue, value);
    }
}

TEST_F(BitBufferTest, testTheTest)
{
    int32_t limitedVal;
    // Test of the tests ;-). It tests the convert function used here in the test.
    for (int32_t i = -8; i < 8; i++) {
        limitedVal = limitValue(i, 4);
        EXPECT_EQ(i, limitedVal);

    }
    limitedVal = limitValue(-9, 4);
    EXPECT_NE(-9, limitedVal);
     limitedVal = limitValue(9, 4);
    EXPECT_NE(9, limitedVal);
}

}  // unnamed namespace
