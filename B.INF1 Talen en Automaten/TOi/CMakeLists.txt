cmake_minimum_required(VERSION 3.30)
project(TA_TOi)

set(CMAKE_CXX_STANDARD 17)

add_executable(TA_TOi main.cpp
		NFA.cpp
        DFA.cpp
        ENFA.cpp
        json.hpp
		General.cpp
		General.h
		RE.cpp
		RE.h)
