cmake_minimum_required(VERSION 2.8)
project(ucineers)

file(GLOB SOURCES "src/**/*.cpp")

add_executable(ucineers ${SOURCES})
set_property(TARGET ucineers PROPERTY CXX_STANDARD 11)
