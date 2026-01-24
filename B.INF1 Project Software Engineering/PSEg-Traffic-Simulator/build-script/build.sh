#!/bin/bash

# Exit on any error
set -e

# File name of the archive
ARCHIVE="gtest-1.7.0-to-recompile-if-needed.tar.xz"

# Step 1: Capture the extracted directory name
dir=$(tar -tf "$ARCHIVE" | head -1 | cut -d/ -f1)

# Step 2: Decompress only if the folder doesn't already exist
if [ ! -d "$dir" ]; then
    tar -xf "$ARCHIVE"
fi

# Step 3: Enter the decompressed directory
cd "$dir"

# Step 4: Configure and build
if [ -f ./configure ]; then
    ./configure
fi

# Use CMake if there's a CMakeLists.txt file
if [ -f CMakeLists.txt ]; then
    mkdir -p build
    cd build
    cmake ..
    make
    cd ..
else
    make
fi

# Step 5: Create a gtest folder two levels up
cd ..
mkdir -p ../gtest

# Step 6: Move include directory to ../gtest
if [ -d "$dir/include" ]; then
    mv "$dir/include" ../gtest/
fi

# Step 7: Create lib folder and move .a files
mkdir -p ../gtest/lib
find "$dir" -name "*.a" -exec mv {} ../gtest/lib/ \;

echo "GTest build and organization complete."

read -p "Do you want to build the main project as well? (y/n): " build_project

if [[ "$build_project" =~ ^[Yy]$ ]]; then
    # Get the script's parent directory
    SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    PARENT_DIR="$(dirname "$SCRIPT_DIR")"

    # Create and enter build directory
    BUILD_DIR="$PARENT_DIR/build"
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    # Run CMake and make
    cmake ..
    make

    # Move release_build and debug_build artifacts
    for artifact in debug_target release_target; do
        if [ -e "$artifact" ]; then
            mv "$artifact" "$PARENT_DIR/"
        fi
    done

    echo "Main project build complete. Artifacts moved to: $PARENT_DIR"
else
    echo "Main project build skipped."
fi

