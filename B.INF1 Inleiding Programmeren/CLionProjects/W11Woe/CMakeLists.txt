cmake_minimum_required(VERSION 3.30)
project(W11Woe)

set(CMAKE_CXX_STANDARD 11)

add_executable(W11Woe main.cpp
        Movie.cpp
        Movie.h
        IMDB.cpp
        IMDB.h
        Director.cpp
        Director.h
        Actor.cpp
        Actor.h)
