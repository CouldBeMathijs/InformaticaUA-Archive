//
// Created by adria on 3-4-2025.
//

#include "Utils.h"

#include <vector>
#include <cmath>
#include <array>

#include "ZBuffer.h"

#include "../ImageGenerator.h"

void Utils::addEdge(std::vector<std::pair<int,int>> &edges, int i, int j) {
    if (j < i) std::swap(i,j);
    edges.emplace_back(i, j);
}

void Utils::draw_zbuf_line(img::EasyImage &image, ZBuffer &zBuffer, int x0, int y0, double z0, int x1, int y1, double z1, const img::Color &color) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int steps = std::max(dx, dy);

    for (int i = 0; i <= steps; ++i) {
        double t = steps == 0 ? 0.0 : static_cast<double>(i) / steps;
        int x = static_cast<int>(std::round((1 - t) * x0 + t * x1));
        int y = static_cast<int>(std::round((1 - t) * y0 + t * y1));
        double invZ = (1 - t) * (1.0 / z0) + t * (1.0 / z1);
        double z = 1.0 / invZ;

        if (zBuffer.testAndSet(x, y, z)) {
            image(x, y) = color;
        }
    }
}

void Utils::draw_zbuf_triangle(img::EasyImage &image, ZBuffer &zBuffer, Vector3D p1, Vector3D p2, Vector3D p3, const img::Color &color) {
    // Sorteer op y zodat p1.y <= p2.y <= p3.y
    if (p2.y < p1.y) std::swap(p1, p2);
    if (p3.y < p1.y) std::swap(p1, p3);
    if (p3.y < p2.y) std::swap(p2, p3);

    int imgWidth = static_cast<int>(image.get_width());
    int imgHeight = static_cast<int>(image.get_height());

    // Bounding box
    int ymin = std::max(0, static_cast<int>(std::floor(p1.y)));
    int ymax = std::min(imgHeight- 1, static_cast<int>(std::ceil(p3.y)));

    double minX = std::min(std::min(p1.x, p2.x), p3.x);
    double maxX = std::max(std::max(p1.x, p2.x), p3.x);

    int xmin = std::max(0, static_cast<int>(std::floor(minX)));
    int xmax = std::min(imgWidth - 1, static_cast<int>(std::ceil(maxX)));

    // Precompute determinants
    double denom = (p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y);
    if (std::abs(denom) < 1e-10) return; // Degenerate triangle

    for (int y = ymin; y <= ymax; ++y) {
        for (int x = xmin; x <= xmax; ++x) {
            // Bereken barycentrische coördinaten
            double lambda1 = ((p2.y - p3.y)*(x - p3.x) + (p3.x - p2.x)*(y - p3.y)) / denom;
            double lambda2 = ((p3.y - p1.y)*(x - p3.x) + (p1.x - p3.x)*(y - p3.y)) / denom;
            double lambda3 = 1.0 - lambda1 - lambda2;

            // Alleen tekenen als het punt binnen de driehoek ligt
            if (lambda1 < 0 || lambda2 < 0 || lambda3 < 0) continue;

            // Interpoleer 1/z
            double invZ = lambda1 / p1.z + lambda2 / p2.z + lambda3 / p3.z;
            double z = 1.0 / invZ;

            // Z-buffer test
            if (zBuffer.testAndSet(x, y, z)) {
                image(x, y) = color;
            }
        }
    }
}
