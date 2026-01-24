#include "WorldAsciiGridParser.h"

#include <stdexcept>

std::vector<std::vector<char>> WorldAsciiGridParser::parse(const std::string& filename) const {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error(filename + " was not opened successfully...");
    }

    std::vector<std::vector<char>> gridData;
    std::string line;

    // 1. Read the file into gridData (Correct: gridData[row][column])
    while (std::getline(inputFile, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        gridData.push_back(std::move(row));
    }

    inputFile.close();

    // Ensure the map is not empty
    if (gridData.empty() || gridData[0].empty()) {
        throw std::runtime_error("World map is empty.");
    }

    // 2. Define dimensions clearly
    const size_t num_cols_x = gridData[0].size(); // X-dimension (width)

    // 3. Validate map consistency
    for (const auto& row : gridData) {
        if (num_cols_x != row.size()) {
            throw std::runtime_error("Invalid world map; every line must have the same number of characters");
        }
    }

    return gridData;
}