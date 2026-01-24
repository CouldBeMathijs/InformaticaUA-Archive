cmake_minimum_required(VERSION 3.26)
project(PSEg_Traffic_Engine)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Set common compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set base flags for all configurations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# Add -O2 only in Release mode
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(./gtest/include)

# Set Library dir
link_directories(./gtest/lib)

# Make sure MacOS finds SMFL
if(APPLE)
    set(SFML_DIR "/usr/local/opt/sfml@2/lib/cmake/SFML")
    find_package(SFML 2.5 REQUIRED graphics window system)
endif()

if(UNIX AND NOT APPLE)
    find_package(SFML 2.5 REQUIRED graphics window system)
endif()

# Set source files for RELEASE target
# Only add cpp files, no h files!
# Only add files with functionality here, no test files!
# Make sure the "main" function is in "main_release"!
set(RELEASE_SOURCE_FILES ./main.cpp
        classes/Simulation.cpp
        classes/Vehicle.cpp
        classes/Stop.cpp
        classes/Road.cpp
        io/input/tinystr.cpp
        io/input/tinyxml.cpp
        io/input/tinyxmlerror.cpp
        io/input/tinyxmlparser.cpp
        classes/VehicleGenerator.cpp
        io/output/SimulatieExporter.cpp
        io/input/XMLInput.cpp
        classes/Intersection.cpp
        io/output/LayoutUtils.cpp
        io/output/3d_rendering/GraphicsExporter.cpp
        io/output/simulation/ResourceManager.cpp
        io/output/simulation/Button.cpp
        io/output/simulation/Camera.cpp
        io/output/simulation/Gui.cpp
        io/output/simulation/SimulationRenderer.cpp
        io/output/simulation/RoadInfoPanel.cpp
        io/output/simulation/ButtonManager.cpp
)

# Set source files for DEBUG target
# Only add cpp files, no h files!
# Make sure the "main" function is in "main_debug"!
set(DEBUG_SOURCE_FILES ./mainTests.cpp
        classes/Simulation.cpp
        classes/Vehicle.cpp
        classes/Stop.cpp
        classes/Road.cpp
        io/input/tinystr.cpp
        io/input/tinyxml.cpp
        io/input/tinyxmlerror.cpp
        io/input/tinyxmlparser.cpp
        classes/VehicleGenerator.cpp
        io/output/SimulatieExporter.cpp
        io/input/XMLInput.cpp
        classes/Intersection.cpp
        io/output/LayoutUtils.cpp
        io/output/3d_rendering/GraphicsExporter.cpp
        io/output/simulation/ResourceManager.cpp
        io/output/simulation/Button.cpp
        io/output/simulation/Camera.cpp
        io/output/simulation/Gui.cpp
        io/output/simulation/SimulationRenderer.cpp
        io/output/simulation/RoadInfoPanel.cpp
        io/output/simulation/ButtonManager.cpp
)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES})

#Build the Graphics Engine
include(ExternalProject)

set(ENGINE_EXECUTABLE_PATH "${CMAKE_SOURCE_DIR}/io/output/3d_rendering/graphics-engine/engine")

if(EXISTS ${ENGINE_EXECUTABLE_PATH})
    message(STATUS "Found existing 'engine' executable at: ${ENGINE_EXECUTABLE_PATH}")
else()
    message(STATUS "'engine' executable not found, building via ExternalProject_Add...")

    ExternalProject_Add(
            graphicsEngine
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/io/output/3d_rendering/graphics-engine
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/graphics_engine_install
            BUILD_ALWAYS 1
    )

    add_dependencies(release_target graphicsEngine)
    add_dependencies(debug_target graphicsEngine)
endif()

# Link libraries
target_link_libraries(debug_target gtest sfml-graphics sfml-window sfml-system)
target_link_libraries(release_target sfml-graphics sfml-window sfml-system)