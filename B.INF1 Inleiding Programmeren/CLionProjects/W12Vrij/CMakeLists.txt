cmake_minimum_required(VERSION 3.30)
project(W12Vrij)

set(CMAKE_CXX_STANDARD 11)

add_executable(W12Vrij main.cpp
        Student.cpp
        Student.h
        Cursus.cpp
        Cursus.h
        Professor.cpp
        Professor.h
        Persoon.cpp
        Persoon.h
        Lesgever.cpp
        Lesgever.h
        Assistent.cpp
        Assistent.h)
