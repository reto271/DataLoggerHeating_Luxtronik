#include <gtest/gtest.h>
#include <stdint.h>

#define __UNIT_TEST_FOR_INFLUX_DB_HPP__
// #define __UNIT_TEST_FOR_INFLUX_DB_DUMP_DBG_INFO_HPP__
#include "MonitorHeating/src/influxdb.hpp"

namespace
{
// using ::testing::StrictMock;
// using ::testing::Return;
// using ::testing::Exactly;
// using ::testing::_;
// using ::testing::HasSubstr;

bool dumpOutput = false;

class Test_influxdb : public testing::Test
{
protected:
    void SetUp() override
    {
        srand(time(NULL));
    }

    void TearDown() override
    {
        // Dump Trace buffer
    }

    void getRandomInt(int& value)
    {
        value = rand() & 0xffffffff;
    }

    void getRandomFloat(double& value, double maxValue)
    {
        value = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxValue));
    }

    void getRandomString(int strSize, std::string& randomString)
    {
        // void randomString(int size, char* output) // pass the destination size and the destination itself
        randomString.resize(strSize);

        // ...and work your way backwards
        while(--strSize > -1) {
            randomString[strSize] = (rand() % 94) + 32; // generate a string ranging from the space character to ~ (tilde)
        }
    }

    uint64_t getTestTime_ns()
    {
        // bool / enum  (1591815558 seconds since the Epoch. Time: Wed Jun 10 20:59:18 2020)
        std::time_t currentUnixTime = std::time(nullptr);
        if(true == dumpOutput) {
            std::cout << "Test time: " << currentUnixTime << " / 0x" << std::hex << currentUnixTime << std::dec << ". Current time: " << std::asctime(std::localtime(&currentUnixTime));
        }
        return static_cast<uint64_t>(1000000000) * currentUnixTime;
    }

    void printStringAndHex(const std::string& testString)
    {
        if(true == dumpOutput) {
            std::cout << "strlen: " << testString.length() << std::endl;
            size_t pos;
            for(pos = 0; pos < testString.length(); pos++) {
                std::cout << std::hex << "0x" << std::setfill('0') << std::setw(2) << (static_cast<uint16_t>(testString[pos]) & 0x00ff) << ", " << std::dec;
                if(0 == ((1 + pos) % 30)) {
                    std::cout << std::endl;
                }

            }
            std::cout << std::endl;
            std::cout << testString << std::endl;
        }
    }

    void printIOVec(const influxdb_cpp::RequestInfo& reqInfo)
    {
        if(true == dumpOutput) {
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
            printStringAndHex(reqInfo.header);
            printStringAndHex(reqInfo.body);
            std::cout << std::endl << "<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        }
    }

};

/// This is not really a unit test. It requires a running influx test data base. You may disable this test if
///  you do not have a test DB instance running. If you have, change IP, port, data base name, user and password.
TEST_F(Test_influxdb, writeToTestDB)
{
    influxdb_cpp::server_info si("192.168.1.100", 22222, "heatingdb_test", "user_test", "password_test");
    // post_http demo with resp[optional]
    std::string resp;
    std::time_t testTime = getTestTime_ns();
    influxdb_cpp::RequestInfo reqInfo;
    int ret = influxdb_cpp::builder()
              .meas("heating_data_test")
              .tag("unit", "kWh")
              .field("TotalEnergy", 100.5, 5)
              .timestamp(testTime)
              .post_http(si, &resp, reqInfo);

    if(true == dumpOutput) {
        std::cout << "ret:  '" << ret << "'" << std::endl;
        std::cout << "resp: '" << resp << "'" << std::endl;
    }

    printIOVec(reqInfo);

    // Check that response is success
    std::string expectedHeader = "POST /write?db=heatingdb_test&u=user_test&p=password_test&epoch=ms HTTP/1.1\r\nHost: 192.168.1.100\r\nContent-Length: 64\r\n\r\n";
    std::string expectedBody = "heating_data_test,unit=kWh TotalEnergy=100.5 " + std::to_string(testTime);
    EXPECT_STREQ(reqInfo.header.c_str(), expectedHeader.c_str());
    EXPECT_STREQ(reqInfo.body.c_str(), expectedBody.c_str());

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE("" == resp);
}

TEST_F(Test_influxdb, ConnectionFails)
{
    // IP address "" and  port 0 is to signal that a unit test is runned. There is no real db server to be used.
    influxdb_cpp::server_info si("", 0, "test_database", "test_user", "test_password");
    // post_http demo with resp[optional]
    std::string resp;
    influxdb_cpp::RequestInfo reqInfo;
    int ret = influxdb_cpp::builder()
              .meas("test_sample")
              .tag("key", "temperature")
              .tag("string", "xxx")
              .field("int", 10)
              .field("longint", 666666)
              .field("float", 10.3, 2)
              .timestamp(1512722735522840439)
              .post_http(si, &resp, reqInfo);

    printIOVec(reqInfo);

    std::string expectedHeader = "POST /write?db=test_database&u=test_user&p=test_password&epoch=ms HTTP/1.1\r\nHost: \r\nContent-Length: 91\r\n\r\n";
    std::string expectedBody = "test_sample,key=temperature,string=xxx int=10i,longint=666666i,float=10 1512722735522840439";

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE("" == resp);
    EXPECT_STREQ(reqInfo.header.c_str(), expectedHeader.c_str());
    EXPECT_STREQ(reqInfo.body.c_str(), expectedBody.c_str());
}

TEST_F(Test_influxdb, DummyConnection)
{
    // IP address "" and  port 0 is to signal that a unit test is runned. There is no real db server to be used.
    influxdb_cpp::server_info si("", 0, "test_data_base", "test_user", "test_password");
    // post_http demo with resp[optional]
    std::string resp;
    influxdb_cpp::RequestInfo reqInfo;
    int ret = influxdb_cpp::builder()
              .meas("test")
              .tag("key", "value")
              .tag("xxx", "yyy")
              .field("x__", 10)
              .field("y__", 10.3, 2)
              .field("b__", !!10)
              .field("a__", "aaa")
              .timestamp(1512722735522840439)
              .post_http(si, &resp, reqInfo);

    if(true == dumpOutput) {
        std::cout << "ret:  '" << ret << "'" << std::endl;
        std::cout << "resp: '" << resp << "'" << std::endl;
    }
    std::string expectedHeader = "POST /write?db=test_data_base&u=test_user&p=test_password&epoch=ms HTTP/1.1\r\nHost: \r\nContent-Length: 69\r\n\r\n";
    std::string expectedBody = "test,key=value,xxx=yyy x__=10i,y__=10,b__=t,a__=t 1512722735522840439";

    EXPECT_EQ(ret, 0);
    EXPECT_TRUE("" == resp);
    EXPECT_STREQ(reqInfo.header.c_str(), expectedHeader.c_str());
    EXPECT_STREQ(reqInfo.body.c_str(), expectedBody.c_str());
}


/// String length calculation is not yet working
TEST_F(Test_influxdb, DISABLED_RandomNumbers)
{
    // IP address "" and  port 0 is to signal that a unit test is runned. There is no real db server to be used.
    influxdb_cpp::server_info si("", 0, "test_data_base", "test_user", "test_password");

    int testInt;
    double testFloat;
    std::string testString;

    getRandomInt(testInt);
    getRandomFloat(testFloat, 1000.0);
    getRandomString(11, testString);

    uint32_t strLenInt = std::to_string(testInt).length();
    uint32_t strLenFloat = std::to_string(testFloat).length();
    uint32_t strLenStr = std::to_string(testInt).length();
    uint32_t totalStrLen = strLenInt + strLenFloat + strLenStr;

    // post_http demo with resp[optional]
    std::string resp;
    influxdb_cpp::RequestInfo reqInfo;
    int ret = influxdb_cpp::builder()
              .meas("test")
              .tag("key", "value")
              .field("int", testInt)
              .field("float", testFloat, 5)
              .field("string", testString)
              .timestamp(1512722735522840439)
              .post_http(si, &resp, reqInfo);

    std::string expectedHeader = "POST /write?db=test_data_base&u=test_user&p=test_password&epoch=ms HTTP/1.1\r\nHost: \r\nContent-Length: "
                                 + std::to_string(54 + totalStrLen) + "\r\n\r\n";
    std::string expectedBody = "test,key=value int=" + std::to_string(testInt)
                               + "i,float=" + std::to_string(testFloat)
                               + ",string=\"" + testString + "\" 1512722735522840439";


    EXPECT_EQ(ret, 0);
    EXPECT_TRUE("" == resp);
    EXPECT_STREQ(reqInfo.header.c_str(), expectedHeader.c_str());
    EXPECT_STREQ(reqInfo.body.c_str(), expectedBody.c_str());
}

}  // unnamed namespace
