cmake_minimum_required(VERSION 3.28)
project(engine)

############################################################
# Set a default build type if none is specified
############################################################
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (default Release)" FORCE)
endif ()

############################################################
# Set compiler flags
############################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Custom flags per configuration
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Wall -Wextra -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -Wextra -std=c++17")

############################################################
# List all sources
############################################################
set(engine_sources
        external/easy_image.cc
        external/easy_image.h
        engine.cc
        external/ini_configuration.cc
        external/l_parser.h
        external/l_parser.cc
        Intro.cpp
        General.cpp
        General.h
        Intro.h
        LSystems2D.cpp
        LSystems2D.h
        external/vector3d.cc
        external/vector3d.h
        Draw3DLines.cpp
        Draw3DLines.h
        external/stb_image.h
        external/ImageComparator.cpp
)

############################################################
# Create an executable
############################################################
set(exe_name "engine")
add_executable(${exe_name} ${engine_sources})

install(TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/)
