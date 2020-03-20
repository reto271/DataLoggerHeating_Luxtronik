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


class Test_BitBuffer : public testing::Test, public BitBuffer
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

    void getRandomValueUnilimted(uint32_t& value, uint16_t& bitWidth)
    {
        // rand returns numbers 0 and RAND_MAX. -> it returns only 31 bit
        assert(RAND_MAX == 0x7fffffff); //
        bitWidth = (rand() % 32) + 1;
        value = (rand() << 1) ^ rand();   // exor two rand variables, shift one
        EXPECT_TRUE(0 < bitWidth);
        EXPECT_TRUE(32 >= bitWidth);
    }

    void getRandomValue(uint32_t& value, uint16_t& bitWidth)
    {
        uint32_t unlimitedVal;
        getRandomValueUnilimted(unlimitedVal, bitWidth);
        uint32_t bitMask = (0x1 << bitWidth) - 1;
        value = unlimitedVal & bitMask;
    }

    void getRandomValue(int32_t& value, uint16_t& bitWidth)
    {
        uint32_t u32value;
        uint32_t bitMask = 0;

        getRandomValue(u32value, bitWidth);
        if (bitWidth == 1) {
            bitWidth++;
        }

        // Expand sign - prepare range check for signed values, not the smartest implementation
        //  but it shall be different from the active code.
        for (uint32_t cnt = 0; cnt < static_cast<uint32_t>(bitWidth-1); cnt++) {
            bitMask |= (0x1 << cnt);;
        }
        if (0 == (0x80000000 & u32value)) {
            // Positive values
            u32value &= bitMask;
        } else {
            // Negative values
            u32value |= (~bitMask);
        }
        value = *reinterpret_cast<int32_t*>(&u32value);
    }

    void testSingleUnsignedValue(uint32_t oriValue, uint8_t nrBits)
    {
        uint32_t testValue;
        this->appendValue(oriValue, nrBits);
        this->getValue(testValue, nrBits);
        EXPECT_EQ(oriValue, testValue);
    }

    void testSingleSignedValue(int32_t oriValue, uint8_t nrBits)
    {
        int32_t testValue;
        this->appendValue(oriValue, nrBits);
        this->getValue(testValue, nrBits);
        EXPECT_EQ(oriValue, testValue);
    }
};


TEST_F(Test_BitBuffer, clearAll32bits)
{
    uint32_t value;

    this->appendValue(static_cast<uint32_t>(0), 32);

    this->restartReading();
    this->getValue(value, 32);

    EXPECT_EQ(0, value);
}

TEST_F(Test_BitBuffer, setAll32bits)
{
    uint32_t value;
    this->appendValue(static_cast<uint32_t>(0xffffffff), 32);

    this->restartReading();
    this->getValue(value, 32);

    EXPECT_EQ(0xffffffff, value);
}

TEST_F(Test_BitBuffer, setGet32BitsAtOnce)
{
    uint32_t value;
    uint32_t value4bits;

    this->appendValue(static_cast<uint32_t>(0x12345678), 32);

    this->restartReading();
    this->getValue(value, 32);
    EXPECT_EQ(0x12345678, value);

    this->restartReading();
    this->getValue(value4bits, 4);
    EXPECT_EQ(value4bits, value & 0xf);
}

TEST_F(Test_BitBuffer, set32BitsAndCheckBuffer)
{
    this->appendValue(static_cast<uint32_t>(0x12345678), 32);

    uint32_t nrBits;
    uint8_t* pBuffer = this->getReferenceToBuffer(nrBits);

    EXPECT_EQ(32, nrBits);
    EXPECT_EQ(0x78, pBuffer[0]);
    EXPECT_EQ(0x56, pBuffer[1]);
    EXPECT_EQ(0x34, pBuffer[2]);
    EXPECT_EQ(0x12, pBuffer[3]);
}

TEST_F(Test_BitBuffer, set5ByteValues)
{
    for (uint32_t cnt = 0; cnt < 5; cnt++) {
        this->appendValue(10 + cnt, 8);
    }

    uint32_t nrBits;
    uint8_t* pBuffer = this->getReferenceToBuffer(nrBits);

    EXPECT_EQ(40, nrBits);
    EXPECT_EQ(10, pBuffer[0]);
    EXPECT_EQ(11, pBuffer[1]);
    EXPECT_EQ(12, pBuffer[2]);
    EXPECT_EQ(13, pBuffer[3]);
    EXPECT_EQ(14, pBuffer[4]);
}


TEST_F(Test_BitBuffer, set34bitValues)
{
    uint32_t value;
    for (uint32_t cnt = 0; cnt < 34; cnt++) {
        this->appendValue(cnt & 0x1, 1);
    }

    uint32_t nrBits;
    uint8_t* pBuffer = this->getReferenceToBuffer(nrBits);

    EXPECT_EQ(34, nrBits);
    EXPECT_EQ(0xaa, pBuffer[0]);
    EXPECT_EQ(0xaa, pBuffer[1]);
    EXPECT_EQ(0xaa, pBuffer[2]);
    EXPECT_EQ(0xaa, pBuffer[3]);
    EXPECT_EQ(0x02, pBuffer[4]);

    this->restartReading();
    for (uint32_t cnt = 0; cnt < 34; cnt++) {
        this->getValue(value, 1);
        EXPECT_EQ(value, (cnt & 0x1));
    }
}

TEST_F(Test_BitBuffer, readWriteSpecificUnsignedValues)
{
    testSingleUnsignedValue(0, 1);
    testSingleUnsignedValue(1, 1);
    //testSingleUnsignedValue(2, 1); -> would assert

    testSingleUnsignedValue(10, 4);
    testSingleUnsignedValue(10, 5);
    testSingleUnsignedValue(10, 32);

    testSingleUnsignedValue(0, 32);
    //testSingleUnsignedValue(0xaffeaffe, 31); -> would assert
    testSingleUnsignedValue(0xaffeaffe, 32);
    testSingleUnsignedValue(0xffffffff, 32);
}

TEST_F(Test_BitBuffer, readWriteSpecificSignedValues)
{
    //testSingleSignedValue(1, 1); -> would assert

    testSingleSignedValue(-1, 2);
    testSingleSignedValue( 0, 2);
    testSingleSignedValue( 1, 2);

    //testSingleSignedValue(10, 4); -> would assert
    testSingleSignedValue(10, 5);
    testSingleSignedValue(10, 10);
    testSingleSignedValue(10, 32);
    //testSingleSignedValue(-10, 4); -> would assert
    testSingleSignedValue(-10, 5);
    testSingleSignedValue(-10, 10);
    testSingleSignedValue(-10, 32);

    testSingleSignedValue(0, 32);
    testSingleSignedValue(0x7fffffff, 32);
    testSingleSignedValue(0xffffffff, 32);
    testSingleSignedValue(0x80000000, 32);
}

TEST_F(Test_BitBuffer, randomBufferReadWriteUnsignedValues)
{
    typedef struct {
        uint32_t randVal;
        uint16_t bitWidth;
    } RandomNumber;
    const uint32_t NrValuesInBuffer = 44;
    RandomNumber randomNumberValues[NrValuesInBuffer];

    // Write values to buffer
    {
        uint32_t value;

        // Write random numbers to buffer and store them in an array
        for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
            uint16_t width;
            getRandomValue(value, width);
            this->appendValue(value, width);
            randomNumberValues[cnt].randVal = value;
            randomNumberValues[cnt].bitWidth = width;
        }
    }

    // Read values back from buffer and expected result array
    {
        uint32_t testValue;
        // Read back values and compare with the values in buffer
        this->restartReading();
        for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
            uint16_t bitWidth = randomNumberValues[cnt].bitWidth;
            this->getValue(testValue, bitWidth);

            // Do not use the function from the buffer on purpose, calculate the bit mask independent
            uint32_t bitMask = (0x1 << bitWidth) - 1;
            if (32 == bitWidth) {
                bitMask = 0xffffffff;
            }
            EXPECT_EQ((randomNumberValues[cnt].randVal & bitMask), testValue );
        }
    }
}

TEST_F(Test_BitBuffer, randomBufferReadWriteSignedValues)
{
    typedef struct {
        int32_t randVal;
        uint16_t bitWidth;
    } RandomNumber;
    const uint32_t NrValuesInBuffer = 100;
    RandomNumber randomNumberValues[NrValuesInBuffer];

    // Write test values to buffer
    {
        int32_t value;

        // Write random numbers to buffer and store them in an array
        for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
            uint16_t width;
            getRandomValue(value, width);

            this->appendValue(value, width);  // To test buffer
            randomNumberValues[cnt].randVal = value; // To array to validate later on
            randomNumberValues[cnt].bitWidth = width;
        }
    }

    // this->printContent();

    {
        int32_t testValue;

        // Read back values and compare with the values in buffer
        this->restartReading();
        for (uint16_t cnt = 0; cnt < NrValuesInBuffer; cnt++) {
            uint16_t bitWidth = randomNumberValues[cnt].bitWidth;
            this->getValue(testValue, bitWidth);
            EXPECT_EQ(randomNumberValues[cnt].randVal, testValue);
        }
    }
}

TEST_F(Test_BitBuffer, testBitMaskForUnsignedValues)
{
    EXPECT_EQ(0x00000001, this->getUnsignedBitMask(1));
    EXPECT_EQ(0x00000003, this->getUnsignedBitMask(2));
    EXPECT_EQ(0x00000007, this->getUnsignedBitMask(3));
    EXPECT_EQ(0x0000000f, this->getUnsignedBitMask(4));
    EXPECT_EQ(0x0000001f, this->getUnsignedBitMask(5));
    EXPECT_EQ(0x0000003f, this->getUnsignedBitMask(6));
    EXPECT_EQ(0x0000007f, this->getUnsignedBitMask(7));
    EXPECT_EQ(0x000000ff, this->getUnsignedBitMask(8));

    EXPECT_EQ(0x000001ff, this->getUnsignedBitMask(9));
    EXPECT_EQ(0x000003ff, this->getUnsignedBitMask(10));
    EXPECT_EQ(0x000007ff, this->getUnsignedBitMask(11));
    EXPECT_EQ(0x00000fff, this->getUnsignedBitMask(12));
    EXPECT_EQ(0x00001fff, this->getUnsignedBitMask(13));
    EXPECT_EQ(0x00003fff, this->getUnsignedBitMask(14));
    EXPECT_EQ(0x00007fff, this->getUnsignedBitMask(15));
    EXPECT_EQ(0x0000ffff, this->getUnsignedBitMask(16));
    EXPECT_EQ(0x0001ffff, this->getUnsignedBitMask(17));
    EXPECT_EQ(0x0003ffff, this->getUnsignedBitMask(18));

    EXPECT_EQ(0x0007ffff, this->getUnsignedBitMask(19));
    EXPECT_EQ(0x000fffff, this->getUnsignedBitMask(20));
    EXPECT_EQ(0x001fffff, this->getUnsignedBitMask(21));
    EXPECT_EQ(0x003fffff, this->getUnsignedBitMask(22));
    EXPECT_EQ(0x007fffff, this->getUnsignedBitMask(23));
    EXPECT_EQ(0x00ffffff, this->getUnsignedBitMask(24));
    EXPECT_EQ(0x01ffffff, this->getUnsignedBitMask(25));
    EXPECT_EQ(0x03ffffff, this->getUnsignedBitMask(26));
    EXPECT_EQ(0x07ffffff, this->getUnsignedBitMask(27));
    EXPECT_EQ(0x0fffffff, this->getUnsignedBitMask(28));

    EXPECT_EQ(0x1fffffff, this->getUnsignedBitMask(29));
    EXPECT_EQ(0x3fffffff, this->getUnsignedBitMask(30));
    EXPECT_EQ(0x7fffffff, this->getUnsignedBitMask(31));
    EXPECT_EQ(0xffffffff, this->getUnsignedBitMask(32));
}

TEST_F(Test_BitBuffer, testBitMaskForSignedValues)
{
    EXPECT_EQ(0x00000001, this->getSignedBitMask(2));
    EXPECT_EQ(0x00000003, this->getSignedBitMask(3));
    EXPECT_EQ(0x00000007, this->getSignedBitMask(4));
    EXPECT_EQ(0x0000000f, this->getSignedBitMask(5));
    EXPECT_EQ(0x0000001f, this->getSignedBitMask(6));
    EXPECT_EQ(0x0000003f, this->getSignedBitMask(7));
    EXPECT_EQ(0x0000007f, this->getSignedBitMask(8));
    EXPECT_EQ(0x000000ff, this->getSignedBitMask(9));

    EXPECT_EQ(0x000001ff, this->getSignedBitMask(10));
    EXPECT_EQ(0x000003ff, this->getSignedBitMask(11));
    EXPECT_EQ(0x000007ff, this->getSignedBitMask(12));
    EXPECT_EQ(0x00000fff, this->getSignedBitMask(13));
    EXPECT_EQ(0x00001fff, this->getSignedBitMask(14));
    EXPECT_EQ(0x00003fff, this->getSignedBitMask(15));
    EXPECT_EQ(0x00007fff, this->getSignedBitMask(16));
    EXPECT_EQ(0x0000ffff, this->getSignedBitMask(17));
    EXPECT_EQ(0x0001ffff, this->getSignedBitMask(18));
    EXPECT_EQ(0x0003ffff, this->getSignedBitMask(19));

    EXPECT_EQ(0x0007ffff, this->getSignedBitMask(20));
    EXPECT_EQ(0x000fffff, this->getSignedBitMask(21));
    EXPECT_EQ(0x001fffff, this->getSignedBitMask(22));
    EXPECT_EQ(0x003fffff, this->getSignedBitMask(23));
    EXPECT_EQ(0x007fffff, this->getSignedBitMask(24));
    EXPECT_EQ(0x00ffffff, this->getSignedBitMask(25));
    EXPECT_EQ(0x01ffffff, this->getSignedBitMask(26));
    EXPECT_EQ(0x03ffffff, this->getSignedBitMask(27));
    EXPECT_EQ(0x07ffffff, this->getSignedBitMask(28));
    EXPECT_EQ(0x0fffffff, this->getSignedBitMask(29));

    EXPECT_EQ(0x1fffffff, this->getSignedBitMask(30));
    EXPECT_EQ(0x3fffffff, this->getSignedBitMask(31));
    EXPECT_EQ(0x7fffffff, this->getSignedBitMask(32));
}

}  // unnamed namespace
