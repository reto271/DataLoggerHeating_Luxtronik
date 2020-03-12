//#include <stdint.h>
//
//class BitBuffer
//{
//public:
//    BitBuffer();
//    ~BitBuffer();
//
//    void appendBits(uint32_t value, uint8_t nrBits);
//
//    void restartReading();
//    uint32_t getBits(uint8_t nrBits);
//
//    uint8_t* getReferenceToBuffer(uint32_t& nrBitsInBuffer);
//
//private:
//    std::vector<uint8_t> m_Buffer;
//};
//

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
        bitWidth = (rand() % 32) + 1;
        value = rand() & 0xffffffff;
        EXPECT_TRUE(0 < bitWidth);
        EXPECT_TRUE(32 >= bitWidth);
    }

    //StrictMock<test::Mock_DummyDriver> m_dummyDriver;
    BitBuffer m_testBuffer;
};


TEST_F(BitBufferTest, clearAll32bits)
{
    m_testBuffer.appendBits(0, 32);

    m_testBuffer.restartReading();
    uint32_t value = m_testBuffer.getBits(32);

    EXPECT_EQ(0, value);
}

TEST_F(BitBufferTest, setAll32bits)
{
    m_testBuffer.appendBits(0xffffffff, 32);

    m_testBuffer.restartReading();
    uint32_t value = m_testBuffer.getBits(32);

    EXPECT_EQ(0xffffffff, value);
}

TEST_F(BitBufferTest, setGet32BitsAtOnce)
{
    m_testBuffer.appendBits(0x12345678, 32);

    m_testBuffer.restartReading();
    uint32_t value = m_testBuffer.getBits(32);
    EXPECT_EQ(0x12345678, value);

    m_testBuffer.restartReading();
    uint32_t value4bits = m_testBuffer.getBits(4);
    EXPECT_EQ(value4bits, value & 0xf);

//    m_testBuffer.printContent();
}

TEST_F(BitBufferTest, set32BitsCheckBuffer)
{
    m_testBuffer.appendBits(0x12345678, 32);

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
        m_testBuffer.appendBits(10 + cnt, 8);
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
    for (uint32_t cnt = 0; cnt < 34; cnt++) {
        m_testBuffer.appendBits(cnt & 0x1, 1);
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
        uint32_t value = m_testBuffer.getBits(1);
        EXPECT_EQ(value , (cnt & 0x1));
    }
}

TEST_F(BitBufferTest, randomBufferReadWrite)
{
    typedef struct{
        uint32_t randVal;
        uint16_t bitWidth;
    } RandomNumber;
    const uint32_t NrValuesInBuffer = 44;
    RandomNumber randomNumberValues[NrValuesInBuffer];

    // Write random numbers to buffer and store them in an array
    for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
        uint32_t value;
        uint16_t width;
        getRandomValue(value, width);
//        std::cout << "Write rand value: " << value << " /  0x" << std::hex << value << std::dec << ", bitWidth: " << width << std::endl;
        m_testBuffer.appendBits(value, width);
        randomNumberValues[cnt].randVal = value;
        randomNumberValues[cnt].bitWidth = width;
    }

    // Read back values and compare with the values in buffer
    m_testBuffer.restartReading();
    for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
        uint32_t value;
        uint16_t bitWidth = randomNumberValues[cnt].bitWidth;
//        std::cout << std::endl << "Read nr Bits: " << bitWidth << std::endl;
        value = m_testBuffer.getBits(bitWidth);

        uint32_t bitMask = (0x1 << bitWidth) - 1;
        if (32 == bitWidth) {
            bitMask = 0xffffffff;
        }

//        std::cout << "bitMask: 0x" << std::hex << bitMask << std::dec << std::endl;
//        std::cout << "Value: " << value << " / 0x" << std::hex << value << std::dec << std::endl;
        EXPECT_EQ((randomNumberValues[cnt].randVal & bitMask), value);

    }
}


}  // unnamed namespace
