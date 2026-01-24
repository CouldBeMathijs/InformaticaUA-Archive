//
// Created by adria on 12-3-2025.
//

#include "Figure3DGenerator.h"
#include "3DUtils/ZBuffer.h"

#include <fstream>
#include <stack>

#include "3DUtils/Transformations.h"

img::EasyImage Figure3DGenerator::generate(const ini::Configuration& config) {
    std::string renderType = config["General"]["type"].as_string_or_die();
    bool useZBuffer = (renderType == "ZBufferedWireframe");
    bool useZBufferTriangles = (renderType == "ZBuffering");

    ini::DoubleTuple bgColor = config["General"]["backgroundcolor"].as_double_tuple_or_die();
    Vector3D eye = Vector3D::point(
        config["General"]["eye"].as_double_tuple_or_die()[0],
        config["General"]["eye"].as_double_tuple_or_die()[1],
        config["General"]["eye"].as_double_tuple_or_die()[2]
    );

    int nrFigures = config["General"]["nrFigures"].as_int_or_die();
    std::vector<Figure3D> figures;

    for (int i = 0; i < nrFigures; i++) {
        std::string figureKey = "Figure" + std::to_string(i);
        std::string type = config[figureKey]["type"].as_string_or_die();

        double rotX = config[figureKey]["rotateX"].as_double_or_default(0);
        double rotY = config[figureKey]["rotateY"].as_double_or_default(0);
        double rotZ = config[figureKey]["rotateZ"].as_double_or_default(0);
        double scaling = config[figureKey]["scale"].as_double_or_default(1.0);
        ini::DoubleTuple centerTup = config[figureKey]["center"].as_double_tuple_or_default({0, 0, 0});
        Vector3D center = Vector3D::point(centerTup[0], centerTup[1], centerTup[2]);
        ini::DoubleTuple colorTup = config[figureKey]["color"].as_double_tuple_or_default({1, 1, 1});
        Vector3D colorVec = Vector3D::vector(colorTup[0], colorTup[1], colorTup[2]);

        Figure3D figure;
        if (type == "LineDrawing") {
            figure = LineDrawingGenerator::createLineDrawing(config, figureKey);
        } else if (type == "3DLSystem") {
            figure = LSystem3DGenerator::create3DLSystemDrawing(config, figureKey);
        } else if (type == "Cube") {
            figure = PrimitiveFigures::createCube();
        } else if (type == "Tetrahedron") {
            figure = PrimitiveFigures::createTetrahedron();
        } else if (type == "Octahedron") {
            figure = PrimitiveFigures::createOctahedron();
        } else if (type == "Icosahedron") {
            figure = PrimitiveFigures::createIcosahedron();
        } else if (type == "Dodecahedron") {
            figure = PrimitiveFigures::createDodecahedron();
        } else if (type == "Cone") {
            int n = config[figureKey]["n"].as_int_or_die();
            double h = config[figureKey]["height"].as_double_or_die();
            figure = PrimitiveFigures::createCone(n, h);
        } else if (type == "Cylinder") {
            int n = config[figureKey]["n"].as_int_or_die();
            double h = config[figureKey]["height"].as_double_or_die();
            figure = PrimitiveFigures::createCylinder(n, h);
        } else if (type == "Sphere") {
            int n = config[figureKey]["n"].as_int_or_die(); // subdiv levels
            figure = PrimitiveFigures::createSphere(n);
        } else if (type == "Torus") {
            double R = config[figureKey]["R"].as_double_or_die(); // grote straal
            double r = config[figureKey]["r"].as_double_or_die(); // kleine straal
            int n = config[figureKey]["n"].as_int_or_die();
            int m = config[figureKey]["m"].as_int_or_die();
            figure = PrimitiveFigures::createTorus(R, r, n, m);
        } else {
            std::cerr << "Onbekend figuur type: " << type << std::endl;
            continue;
        }

        figure.color = colorVec;

        Matrix M = Transformations::scaleMatrix(scaling)
                   * Transformations::rotateXMatrix(rotX)
                   * Transformations::rotateYMatrix(rotY)
                   * Transformations::rotateZMatrix(rotZ)
                   * Transformations::translateMatrix(center);

        for (auto &p: figure.points) {
            p *= M;
        }

        figures.push_back(std::move(figure));
    }

    return drawFigures(config, figures, useZBuffer, useZBufferTriangles);
}

img::EasyImage Figure3DGenerator::drawFigures(const ini::Configuration& config, const std::vector<Figure3D>& figuresInput, bool useZBuffer, bool useZBufferTriangles) {
    if (figuresInput.empty()) {
        std::cerr << "Not generating image, figures.size() == 0\n";
        return img::EasyImage();
    }

    int size = config["General"]["size"].as_int_or_die();
    ini::DoubleTuple bgColor = config["General"]["backgroundcolor"].as_double_tuple_or_die();
    Vector3D eye = Vector3D::point(
        config["General"]["eye"].as_double_tuple_or_die()[0],
        config["General"]["eye"].as_double_tuple_or_die()[1],
        config["General"]["eye"].as_double_tuple_or_die()[2]
    );

    auto figures = figuresInput; // copy so we can mutate
    projectAndTransformFigures(figures, eye);

    double minX, maxX, minY, maxY;
    calculateBoundingBox(figures, minX, maxX, minY, maxY);

    int imageWidth, imageHeight;
    double scaleFactor, offsetX, offsetY;
    computeImageParams(size, minX, maxX, minY, maxY, imageWidth, imageHeight, scaleFactor, offsetX, offsetY);

    img::EasyImage image = createImage(imageWidth, imageHeight, bgColor);
    ZBuffer zBuffer(imageWidth, imageHeight);

    for (const Figure3D &figure : figures) {
        drawFigureLines(image, figure, minX, minY, scaleFactor, offsetX, offsetY, zBuffer, useZBuffer, useZBufferTriangles);
    }

    return image;
}

void Figure3DGenerator::applyTransformation(std::vector<Vector3D> &points, const Matrix &m) {
    for (auto &point : points) {
        point *= m;
    }
}

void Figure3DGenerator::projectAndTransformFigures(std::vector<Figure3D> &figures, const Vector3D &eye) {
    Matrix projectionMatrix = Transformations::eyePointTrans(eye);
    double d = -sqrt(eye.x * eye.x + eye.y * eye.y + eye.z * eye.z);

    for (Figure3D &figure : figures) {
        applyTransformation(figure.points, projectionMatrix);
        for (Vector3D &point : figure.points) {
            point = Transformations::doProjection(point, d);
        }
    }
}

void Figure3DGenerator::calculateBoundingBox(const std::vector<Figure3D> &figures,
                                             double &minX, double &maxX, double &minY, double &maxY) {
    minX = maxX = figures[0].points[0].x;
    minY = maxY = figures[0].points[0].y;

    for (const Figure3D &figure : figures) {
        for (const Vector3D &point : figure.points) {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }
    }
}

void Figure3DGenerator::computeImageParams(int size, double minX, double maxX, double minY, double maxY, int &width, int &height, double &scaleFactor, double &offsetX, double &offsetY) {
    double w = maxX - minX;
    double h = maxY - minY;
    bool widthIsBigger = (w >= h);

    scaleFactor = (0.95 * size) / (widthIsBigger ? w : h);
    double finalW = widthIsBigger ? size : (w * scaleFactor) / 0.95;
    double finalH = widthIsBigger ? (h * scaleFactor) / 0.95 : size;

    width = static_cast<int>(std::floor(finalW));
    height = static_cast<int>(std::floor(finalH));
    offsetX = (width - scaleFactor * w) / 2;
    offsetY = (height - scaleFactor * h) / 2;
}

img::EasyImage Figure3DGenerator::createImage(int width, int height, const ini::DoubleTuple &bgColor) {
    return img::EasyImage(width, height, img::Color(
        bgColor[0] * 255, bgColor[1] * 255, bgColor[2] * 255
    ));
}

void Figure3DGenerator::drawFigureLines(img::EasyImage &image, const Figure3D &figure,
                                        double minX, double minY, double scaleFactor,
                                        double offsetX, double offsetY,
                                        ZBuffer &zBuffer, bool useZBuffer, bool useZBufferTriangles) {
    img::Color color(figure.color.x * 255, figure.color.y * 255, figure.color.z * 255);

    if (useZBufferTriangles) {
        for (const auto &face : figure.faces) {
            // Create transformed points with the same scaling and offset as the wireframe
            Vector3D p1 = figure.points[face[0]];
            Vector3D p2 = figure.points[face[1]];
            Vector3D p3 = figure.points[face[2]];

            // Apply the same scaling and offset as used for the lines
            p1.x = scaleFactor * (p1.x - minX) + offsetX;
            p1.y = scaleFactor * (p1.y - minY) + offsetY;

            p2.x = scaleFactor * (p2.x - minX) + offsetX;
            p2.y = scaleFactor * (p2.y - minY) + offsetY;

            p3.x = scaleFactor * (p3.x - minX) + offsetX;
            p3.y = scaleFactor * (p3.y - minY) + offsetY;

            Utils::draw_zbuf_triangle(image, zBuffer, p1, p2, p3, color);
        }
        return;
    }

    for (const auto &line : figure.lines) {
        int x0 = static_cast<int>(scaleFactor * (figure.points[line.first].x - minX) + offsetX);
        int y0 = static_cast<int>(scaleFactor * (figure.points[line.first].y - minY) + offsetY);
        int x1 = static_cast<int>(scaleFactor * (figure.points[line.second].x - minX) + offsetX);
        int y1 = static_cast<int>(scaleFactor * (figure.points[line.second].y - minY) + offsetY);

        double z0 = figure.points[line.first].z;
        double z1 = figure.points[line.second].z;

        if (useZBuffer) {
            Utils::draw_zbuf_line(image, zBuffer, x0, y0, z0, x1, y1, z1, color);
        } else {
            image.draw_line(x0, y0, x1, y1, color);
        }
    }
}