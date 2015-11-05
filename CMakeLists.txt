cmake_minimum_required(VERSION 2.8)
project(uccineers)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")

file(GLOB SOURCES "src/*.cpp")
file(GLOB COMMON_SOURCES "src/common/*.cpp")

include_directories("src/common")
add_executable(uccineers "src/uccineers.cpp" ${COMMON_SOURCES} ${SOURCES})
