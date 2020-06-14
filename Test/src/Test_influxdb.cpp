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

    void  getRandomValue(uint32_t& value)
    {
        value = rand() & 0xffffffff;
    }

    void  getRandomValue(double & value, double maxValue)
    {
        value = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / maxValue));
    }

    uint64_t getTestTime_ns()
    {
        // bool / enum  (1591815558 seconds since the Epoch. Time: Wed Jun 10 20:59:18 2020)
        std::time_t currentUnixTime = std::time(nullptr);
        std::cout << "Test time: " << currentUnixTime << " / 0x" << std::hex << currentUnixTime << std::dec << ". Current time: " << std::asctime(std::localtime(&currentUnixTime));
        return static_cast<uint64_t>(1000000000) * currentUnixTime;
    }

};


// TEST_F(Test_influxdb, test01)
// {
//    uint32_t value;
//    this->getRandomValue(value);
//
//    double fValue;
//    this->getRandomValue(fValue, 100.0);
//
//    EXPECT_EQ(0, value);
//    EXPECT_EQ(0, fValue);
// }
//
// TEST_F(Test_influxdb, test02)
// {
// #if 0
//    // Make sure it does not write to the productive DB
//    influxdb_cpp::server_info si("192.168.2.100", 8086, "heatingdb-test", "", "");
//
//    // bool / enum  (1591815558 seconds since the Epoch. Time: Wed Jun 10 20:59:18 2020)
//    uint64_t time_ns = getTestTime_ns();
//
//    influxdb_cpp::builder().meas("heating_data").tag("unit", "bool").field("EVU Sperre", "1").timestamp(time_ns).post_http(si);
//
//    // double
//    influxdb_cpp::builder().meas("heating_data").tag("unit", "kWh").field("Total Ele Energy", 1000.04, 5).timestamp(time_ns).post_http(si);
// #endif
//    EXPECT_FALSE(true);
// }


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

TEST_F(Test_influxdb, DISABLED_writeToTestDB)
{
    influxdb_cpp::server_info si("192.168.1.100", 22222, "heatingdb_test", "user_test", "password_test");
    // post_http demo with resp[optional]
    std::string resp;
//  influxdb_cpp::builder().meas("heating_data").tag("unit", finalUnit).field(description, intFinalValue).timestamp(time_ns).post_http(si);
    influxdb_cpp::RequestInfo reqInfo;
    int ret = influxdb_cpp::builder()
              .meas("heating_data_test")
              .tag("unit", "kWh")
              .field("TotalEnergy", 100.5, 5)
              .timestamp(getTestTime_ns())
              .post_http(si, &resp, reqInfo);

    if(true == dumpOutput) {
        std::cout << "ret:  '" << ret << "'" << std::endl;
        std::cout << "resp: '" << resp << "'" << std::endl;
    }

    // printIOVec(ioVector[0]);
    // printIOVec(ioVector[1]);
    // void* iov_base;
    // size_t iov_len;


    // Check that response is success
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE("" == resp);
}

TEST_F(Test_influxdb, ConnectionFails)
{
    // IP address "" and  port 0 is to signal that a unit test is runned. There is no real server to be used.
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
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE("" == resp);

    // POST /write?db=test_database&u=test_user&p=test_password&epoch=ms HTTP/1.1
    // Host:
    // Content-Length: 91
    std::string expectedHeader = "POST /write?db=test_database&u=test_user&p=test_password&epoch=ms HTTP/1.1\r\nHost: \r\nContent-Length: 91\r\n\r\n";
    EXPECT_STREQ(reqInfo.header.c_str(), expectedHeader.c_str());

    // test_sample,key=temperature,string=xxx int=10i,longint=666666i,float=10 1512722735522840439
    std::string expectedBody = "test_sample,key=temperature,string=xxx int=10i,longint=666666i,float=10 1512722735522840439";
    EXPECT_STREQ(reqInfo.body.c_str(), expectedBody.c_str());
}

TEST_F(Test_influxdb, DISABLED_DummyConnection)
{
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

    std::cout << "ret:  '" << ret << "'" << std::endl;
    std::cout << "resp: '" << resp << "'" << std::endl;


    EXPECT_EQ(ret, -3);
    EXPECT_TRUE("" == resp);
}

}  // unnamed namespace
