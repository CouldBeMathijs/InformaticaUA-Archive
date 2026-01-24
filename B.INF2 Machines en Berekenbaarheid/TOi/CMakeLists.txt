cmake_minimum_required(VERSION 3.30)
project(Programmeeropdracht)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG           "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE         "-O0")
set(CMAKE_CXX_FLAGS_MINSIZEREL      "-O0")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# ------------------------------------------------------------------------

add_executable(Programmeeropdracht main.cpp
        CFG.cpp
        CFG.h
        json.hpp
        PDA.cpp
        PDA.h)
