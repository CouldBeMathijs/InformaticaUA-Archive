cmake_minimum_required(VERSION 3.30)
project(W12Vrij)

set(CMAKE_CXX_STANDARD 11)

add_executable(W12Vrij main.cpp
        Oef1/Student.cpp
        Oef1/Student.h
        Oef1/Cursus.cpp
        Oef1/Cursus.h
        Oef1/Professor.cpp
        Oef1/Professor.h
        Oef1/Persoon.cpp
        Oef1/Persoon.h
        Oef1/Lesgever.cpp
        Oef1/Lesgever.h
        Oef1/Assistent.cpp
        Oef1/Assistent.h
        Oef2/Movie.cpp)
