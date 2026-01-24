#!/bin/bash

# Define the output zip file
output_file="poging.zip"

# Find all .cpp files excluding main.cpp and all .h files, then zip them
zip "$output_file" $(find . -maxdepth 1 -type f \( -name "*.cpp" ! -name "main.cpp" -o -name "*.h" \))

# Confirm the result
if [ $? -eq 0 ]; then
    echo "Done :)"
else
    echo "An error occurred while zipping the files."
fi
