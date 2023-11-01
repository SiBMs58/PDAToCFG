cmake_minimum_required(VERSION 3.26)
project(PDAToCFG)

set(CMAKE_CXX_STANDARD 14)

add_executable(PDAToCFG
        main.cpp
        CFG.cpp
        PDA.cpp)
