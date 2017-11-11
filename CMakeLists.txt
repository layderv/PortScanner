cmake_minimum_required(VERSION 3.8)
project(PortScanner)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp scanner.cpp scanner.h errors.h)
add_executable(PortScanner ${SOURCE_FILES})