//
// Created by adria on 3-4-2025.
//

#include "ZBuffer.h"

#include <cmath>
#include <limits>

ZBuffer::ZBuffer(int width, int height)
    : width(width), height(height),
      buffer(height, std::vector<double>(width, std::numeric_limits<double>::infinity())) {}

bool ZBuffer::testAndSet(int x, int y, double z) {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;

    double invZ = 1.0 / ((std::abs(z) < 1e-6) ? 1e-6 : z);
    if (invZ < buffer[y][x]) {
        buffer[y][x] = invZ;
        return true;
    }
    return false;
}

void ZBuffer::clear() {
    for (auto& row : buffer) {
        std::fill(row.begin(), row.end(), std::numeric_limits<double>::infinity());
    }
}