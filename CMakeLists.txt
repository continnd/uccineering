cmake_minimum_required(VERSION 2.8)
project(ucineers)

file(GLOB SOURCES "src/*.cpp")
file(GLOB COMMON_SOURCES "src/common/*.cpp")

include_directories("src/common")
add_executable(ucineers "src/ucineers.cpp" ${COMMON_SOURCES} ${SOURCES})
set_property(TARGET ucineers PROPERTY CXX_STANDARD 11)
