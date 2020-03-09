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
        //Create and initialize trace infrastructure
    }

    void TearDown() override
    {
        //Dump Trace buffer
    }

    //StrictMock<test::Mock_DummyDriver> m_dummyDriver;
    BitBuffer m_testBuffer;
};


TEST_F(BitBufferTest, setGet32BitsAtOnce)
{
    m_testBuffer.appendBits(0x12345678, 32);

    m_testBuffer.restartReading();
    uint32_t value = m_testBuffer.getBits(32);

    EXPECT_EQ(0x12345678, value);
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

}  // unnamed namespace
