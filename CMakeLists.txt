cmake_minimum_required(VERSION 3.25)
project(ALOGGER)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(BIN
    src/test.cpp
    src/Logger.cpp
)
