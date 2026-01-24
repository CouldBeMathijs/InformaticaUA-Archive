cmake_minimum_required(VERSION 3.30)
project(TinyXML)

set(CMAKE_CXX_STANDARD 20)

add_executable(main.cpp tinystr.cpp
        main.cpp
        tinyxmlerror.cpp
        tinyxml.cpp
        tinyxmlparser.cpp)
