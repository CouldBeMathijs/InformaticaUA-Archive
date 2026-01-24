//
// Created by adria on 3-4-2025.
//

#ifndef UTILS_H
#define UTILS_H
#include <vector>

#include "ZBuffer.h"
#include "../ImageGenerator.h"
#include "../../external/vector3d.h"


class Utils {
public:
    static void addEdge(std::vector<std::pair<int,int>> &edges, int i, int j);

    static void draw_zbuf_line(img::EasyImage &image, ZBuffer &zBuffer, int x0, int y0, double z0, int x1, int y1, double z1, const img::Color &color);

    static void draw_zbuf_triangle(img::EasyImage &image, ZBuffer &zBuffer, Vector3D p1, Vector3D p2, Vector3D p3, const img::Color &color);
};


#endif //UTILS_H
