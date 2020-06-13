#include <gtest/gtest.h>
#include <stdint.h>

#define __UNIT_TEST_FOR_INFLUX_DB_HPP__
#include "MonitorHeating/src/influxdb.hpp"

namespace
{
// using ::testing::StrictMock;
// using ::testing::Return;
// using ::testing::Exactly;
// using ::testing::_;
// using ::testing::HasSubstr;


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

    bool dumpOutput = false;
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

void printIOVec(iovec& iov)
{
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
    std::cout << "ioVec len : " << iov.iov_len << std::endl;

    for(size_t pos = 0; pos < iov.iov_len; pos++) {
        std::cout << std::hex << "0x" << std::setfill('0') << std::setw(2) << (0xff & static_cast<uint16_t>(static_cast<char*>(iov.iov_base)[pos])) << ", " << std::dec;
        if(0 == ((1 + pos) % 30)) {
            std::cout << std::endl;
        }

    }
    std::cout << std::endl;
    for(size_t pos = 0; pos < iov.iov_len; pos++) {
        std::cout << static_cast<char*>(iov.iov_base)[pos] << std::dec;
//        if(0 == ((1+pos) % 30)) {
//            std::cout << std::endl;
//        }

    }
    std::cout << std::endl << "<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
}

TEST_F(Test_influxdb, writeToTestDB)
{
    influxdb_cpp::server_info si("192.168.1.100", 22222, "heatingdb_test", "test", "test");
    // post_http demo with resp[optional]
    std::string resp;
//  influxdb_cpp::builder().meas("heating_data").tag("unit", finalUnit).field(description, intFinalValue).timestamp(time_ns).post_http(si);
    iovec ioVector[2];
    int ret = influxdb_cpp::builder()
              .meas("heating_data_test")
              .tag("unit", "kWh")
              .field("TotalEnergy", 100.5, 5)
              .timestamp(getTestTime_ns())
              .post_http(si, &resp, ioVector);

    if(true == dumpOutput) {
        std::cout << "ret:  '" << ret << "'" << std::endl;
        std::cout << "resp: '" << resp << "'" << std::endl;
    }

    printIOVec(ioVector[0]);
    printIOVec(ioVector[1]);
    // void* iov_base;
    // size_t iov_len;


    // Check that response is success
    EXPECT_EQ(ret, 0);
    EXPECT_TRUE("" == resp);
}

TEST_F(Test_influxdb, DISABLED_ConnectionFails)
{
    influxdb_cpp::server_info si("127.0.0.1", 1, "testx", "test", "test");
    // post_http demo with resp[optional]
    std::string resp;
    iovec ioVector[2];
    int ret = influxdb_cpp::builder()
              .meas("test")
              .tag("key", "value")
              .tag("xxx", "yyy")
              .field("x__", 10)
              .field("y__", 10.3, 2)
              .field("b__", !!10)
              .field("a__", "aaa")
              .timestamp(1512722735522840439)
              .post_http(si, &resp, ioVector);

    std::cout << "ret:  '" << ret << "'" << std::endl;
    std::cout << "resp: '" << resp << "'" << std::endl;

    EXPECT_EQ(ret, -3);
    EXPECT_TRUE("" == resp);
}

TEST_F(Test_influxdb, DISABLED_DummyConnection)
{
    influxdb_cpp::server_info si("", 0, "test_data_base", "test_user", "test_password");
    // post_http demo with resp[optional]
    std::string resp;
    iovec ioVector[2];
    int ret = influxdb_cpp::builder()
              .meas("test")
              .tag("key", "value")
              .tag("xxx", "yyy")
              .field("x__", 10)
              .field("y__", 10.3, 2)
              .field("b__", !!10)
              .field("a__", "aaa")
              .timestamp(1512722735522840439)
              .post_http(si, &resp, ioVector);

    std::cout << "ret:  '" << ret << "'" << std::endl;
    std::cout << "resp: '" << resp << "'" << std::endl;

    EXPECT_EQ(ret, -3);
    EXPECT_TRUE("" == resp);
}

}  // unnamed namespace
