//
// Created by adria on 12-3-2025.
//

#ifndef FIGURE3DGENERATOR_H
#define FIGURE3DGENERATOR_H
#define _USE_MATH_DEFINES

#include "ImageGenerator.h"
#include "3DUtils/PrimitiveFigures.h"
#include "3DUtils/LSystem3DGenerator.h"
#include "3DUtils/LineDrawingGenerator.h"
#include "3DUtils/Transformations.h"

#include "../external/vector3d.h"
#include "../external/l_parser.h"

#include <cmath>
#include <vector>
#include <array>

#include "3DUtils/ZBuffer.h"

struct Figure3D {
    std::vector<Vector3D> points;
    std::vector<std::pair<int, int>> lines;
    std::vector<std::array<int, 3>> faces;
    double rotateX, rotateY, rotateZ;
    double scale;
    Vector3D center;
    Vector3D color;
};

class Figure3DGenerator : public ImageGenerator {
public:
    img::EasyImage generate(const ini::Configuration& config) override;
    static img::EasyImage drawFigures(const ini::Configuration& config, const std::vector<Figure3D>& figures, bool useZBuffer, bool useZBufferTriangles);

private:

    static void applyTransformation(std::vector<Vector3D> &points, const Matrix &m);

    static void projectAndTransformFigures(std::vector<Figure3D> &figures, const Vector3D &eye);

    static void calculateBoundingBox(const std::vector<Figure3D> &figures, double &minX, double &maxX, double &minY, double &maxY);

    static void computeImageParams(int size, double minX, double maxX, double minY, double maxY, int &width, int &height, double &scaleFactor, double &offsetX, double &offsetY);

    static img::EasyImage createImage(int width, int height, const ini::DoubleTuple &bgColor);

    static void drawFigureLines(img::EasyImage &image, const Figure3D &figure, double minX, double minY, double scaleFactor, double offsetX, double offsetY, ZBuffer &zBuffer, bool useZBuffer, bool useZBufferTriangles);
};

#endif //FIGURE3DGENERATOR_H
