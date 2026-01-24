cmake_minimum_required(VERSION 3.30)
project(oefenexamencpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(oefenexamencpp main.cpp
        Time.cpp
        Time.h
        Audiobestand.cpp
        Audiobestand.h
        Mediaplayer.cpp
        Mediaplayer.h)
