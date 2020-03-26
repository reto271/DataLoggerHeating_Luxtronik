#include <gtest/gtest.h>
#include <stdint.h>

#include "Common/src/FeedbackCollector.hpp"

namespace
{
// using ::testing::StrictMock;
// using ::testing::Return;
// using ::testing::Exactly;
// using ::testing::_;
// using ::testing::HasSubstr;


class Test_FeedbackCollector : public testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

};


TEST_F(Test_FeedbackCollector, initializeFalse)
{
    FeedbackCollector test(false);
    EXPECT_FALSE(test.getFeedback());
}

TEST_F(Test_FeedbackCollector, initializeTrue)
{
    FeedbackCollector test(true);
    EXPECT_TRUE(test.getFeedback());
}

TEST_F(Test_FeedbackCollector, testAnd)
{
    FeedbackCollector test(true);
    EXPECT_TRUE(test.getFeedback());
    test.addAndFeedback(true);
    EXPECT_TRUE(test.getFeedback());
    test.addAndFeedback(false);
    EXPECT_FALSE(test.getFeedback());
    // Remains false
    test.addAndFeedback(true);
    EXPECT_FALSE(test.getFeedback());
    test.addAndFeedback(false);
    EXPECT_FALSE(test.getFeedback());
}

TEST_F(Test_FeedbackCollector, testOr)
{
    FeedbackCollector test(false);
    EXPECT_FALSE(test.getFeedback());
    test.addOrFeedback(false);
    EXPECT_FALSE(test.getFeedback());
    test.addOrFeedback(true);
    EXPECT_TRUE(test.getFeedback());
    // Remains true
    test.addOrFeedback(true);
    EXPECT_TRUE(test.getFeedback());
    test.addOrFeedback(false);
    EXPECT_TRUE(test.getFeedback());
}

}  // unnamed namespace
