cmake_minimum_required(VERSION 2.6.4)
project(UnixTimeConverter)

enable_testing()
#add_subdirectory(googletest)

# Define compiler switches
#set(CMAKE_CXX_FLAGS "-ggdb -g3 -Dgmock_build_tests=ON -DUNIT_TESTS_SPM -Wall")
set(CMAKE_CXX_FLAGS "-ggdb -g3 -Wall")

# This sets the output dir to /bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#include_directories(${CMAKE_SOURCE_DIR}/googletest/include)
add_executable(UnixTimeConverter src/testConvertTime.cpp)
target_link_libraries(UnixTimeConverter)
