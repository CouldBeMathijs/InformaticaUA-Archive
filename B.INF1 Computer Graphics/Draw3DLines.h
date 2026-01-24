//
// Created by mathijs on 05/03/25.
//

#ifndef DRAW3DLINES_H
#define DRAW3DLINES_H
#include "external/easy_image.h"
#include "external/vector3d.h"

namespace ini {
    class Configuration;
}


struct Figure;

Matrix scaleFigure(double scale);

Matrix rotateX(double angle);

Matrix rotateY(double angle);

Matrix rotateZ(double angle);

Matrix translate(const Vector3D &vector);

Matrix translate(const std::vector<double> &vector);

Figure createCube();

Figure createTetrahedron();

Figure createOctahedron();

Figure createDodecahedron();

Figure createIcosahedron();

Figure createSphere(unsigned int);

Figure createCone(double h, unsigned int n);

Figure createCylinder(double h, unsigned int n);

Figure createTorus(double r, double R, int n, int m);

Figure create3DLsystem(const std::string &inputFile);

void applyTransformation(std::vector<Figure> &fig, const Matrix &m);

Matrix eyePointTrans(const Vector3D &eyepoint);

void toPolar(const Vector3D &point, double &theta, double &phi, double &r);

std::vector<img::Line2D> doProjectionNoZBuf(const std::vector<Figure> &figures);

bool drawZBuffering(const ini::Configuration &conf, img::EasyImage &image);

bool drawZBufferedWireframe(const ini::Configuration &conf, img::EasyImage &image);

bool drawWireframe(const ini::Configuration &conf, img::EasyImage &image);

#endif //DRAW3DLINES_H
