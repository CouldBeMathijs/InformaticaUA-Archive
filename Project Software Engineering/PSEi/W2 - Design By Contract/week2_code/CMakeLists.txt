cmake_minimum_required(VERSION 3.6)
project(ProjectTitle)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
# Remove the "-O2" flag if you have problems with the debugger.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(./gtest/include)

# Set Library dir
link_directories(./gtest/lib)

# Set source files for RELEASE target
# Only add cpp files, no h files!
# Only add files with functionality here, no test files!
# Make sure the "main" function is in "main_release"!
set(RELEASE_SOURCE_FILES ./src/EXAMPLE1.cpp ./src/EXAMPLE2.cpp ./src/main_release.cpp)

# Set source files for DEBUG target
# Only add cpp files, no h files!
# Make sure the "main" function is in "main_debug"!
set(DEBUG_SOURCE_FILES ./src/EXAMPLE1.cpp ./src/EXAMPLE2.cpp ./src/EXAMPLE_TESTS1.cpp ./src/EXAMPLE_TESTS1.cpp  ./src/main_debug.cpp)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(debug_target gtest)

