cmake_minimum_required(VERSION 2.6.4)
project(DataDecoder)

enable_testing()
#add_subdirectory(googletest)

# Define compiler switches
#set(CMAKE_CXX_FLAGS "-ggdb -g3 -Dgmock_build_tests=ON -DUNIT_TESTS_SPM -Wall")
set(CMAKE_CXX_FLAGS "-ggdb -g3 -Wall")

# This sets the output dir to /bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/..)
add_executable(DataDecoder
               src/DataDecoder_main.cpp
               src/FileDataReader.cpp
               ../Common/src/ValueTableCommon.cpp
               ../Common/src/ValueTable.cpp
               ../Common/src/ValueTable_v1.cpp
               ../Common/src/ValueTable_v2.cpp
               ../Common/src/BitBuffer.cpp
               ../Common/src/FeedbackCollector.cpp)
target_link_libraries(DataDecoder)
