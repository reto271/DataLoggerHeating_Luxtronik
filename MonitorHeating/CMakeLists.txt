cmake_minimum_required(VERSION 2.6.4)
project(MonitorHeating)

enable_testing()
#add_subdirectory(googletest)

# Define compiler switches
#set(CMAKE_CXX_FLAGS "-ggdb -g3 -Dgmock_build_tests=ON -DUNIT_TESTS_SPM -Wall -std=c++17")
set(CMAKE_CXX_FLAGS "-ggdb -g3 -Wall -std=c++17")

# This sets the output dir to /bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/..)
add_executable(MonitorHeating
                src/MonitorHeating_main.cpp
                src/TcpConnection.cpp
                src/ReceiveDataBuffer.cpp
                src/ValueResponse.cpp
                src/SynchronizeTime.cpp
                ../Common/src/BitBuffer.cpp
                ../Common/src/IP_AddressValidator.cpp
                ../Common/src/ValueTableCommon.cpp
                ../Common/src/ValueTable.cpp)

target_link_libraries(MonitorHeating)
