cmake_minimum_required(VERSION 3.30)
project(W11Vrij)

set(CMAKE_CXX_STANDARD 11)

add_executable(W11Vrij main.cpp
        VeeltermFunctie.cpp
        VeeltermFunctie.h
        RationaleFunctie.cpp
        RationaleFunctie.h
        Geogebra.cpp
        Geogebra.h)
