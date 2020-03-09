#include <gtest/gtest.h>
#include <stdint.h>

namespace
{
// using ::testing::StrictMock;
// using ::testing::Return;
// using ::testing::Exactly;
// using ::testing::_;
// using ::testing::HasSubstr;


class DummyTest : public testing::Test
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
};


TEST_F(DummyTest, successful)
{
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}


TEST_F(DummyTest, failure)
{
    //EXPECT_CALL(m_lmsSpmDriver, WaitOnIRQ()).Times(Exactly(1));
    EXPECT_EQ(1, 2);
}



}  // unnamed namespace
