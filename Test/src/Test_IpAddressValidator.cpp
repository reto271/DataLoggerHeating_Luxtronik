#include <gtest/gtest.h>
#include <stdint.h>

#include "Common/src/IP_AddressValidator.hpp"

namespace
{
// using ::testing::StrictMock;
// using ::testing::Return;
// using ::testing::Exactly;
// using ::testing::_;
// using ::testing::HasSubstr;


class Test_IpAddressValidator : public testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

};


TEST_F(Test_IpAddressValidator, testSomoeLocalIP)
{
    {
        IP_AddressValidator testIp("192.168.1.0");
        EXPECT_TRUE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("192.168.1.1");
        EXPECT_TRUE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("192.168.1.2");
        EXPECT_TRUE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("192.168.1.3");
        EXPECT_TRUE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("192.168.1.100");
        EXPECT_TRUE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("192.168.1.255");
        EXPECT_TRUE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255.255.255.0");
        EXPECT_TRUE(testIp.validate());
    }
}

TEST_F(Test_IpAddressValidator, rangeOfIP)
{
    for(int w = 0; w < 10; w++) {
        for(int x = 250; x < 255; x++) {
            for(int y = 100; y < 110; y++) {
                for(int z = 0; z < 256; z++) {
                    std::string ip = std::to_string(w) + "." + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
                    // std::cout << "Test IP: " << ip << std::endl;
                    IP_AddressValidator testIp(ip);
                    EXPECT_TRUE(testIp.validate());
                }
            }
        }
    }
}

TEST_F(Test_IpAddressValidator, outOfRange)
{
    {
        IP_AddressValidator testIp("255.255.255.256");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255.255.255.1000");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255.255.255.-1");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("333.333.333.333");
        EXPECT_FALSE(testIp.validate());
    }

}
TEST_F(Test_IpAddressValidator, someInvalidIP)
{
    {
        IP_AddressValidator testIp("255.255.255");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255.255.255,0");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255.255");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255.255.255..200");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255.");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("255");
        EXPECT_FALSE(testIp.validate());
    }
    {
        IP_AddressValidator testIp("a.b.c.d");
        EXPECT_FALSE(testIp.validate());
    }
}

}  // unnamed namespace
