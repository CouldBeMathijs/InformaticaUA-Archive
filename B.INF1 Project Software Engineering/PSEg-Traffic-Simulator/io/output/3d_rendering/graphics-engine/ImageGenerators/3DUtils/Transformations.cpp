//
// Created by adria on 3-4-2025.
//

#include "Transformations.h"

Matrix Transformations::scaleMatrix(double scale) {
    Matrix m;
    m(1, 1) = scale;
    m(2, 2) = scale;
    m(3, 3) = scale;
    return m;
}

Matrix Transformations::rotateXMatrix(double angle) {
    Matrix m;
    double rad = angle * M_PI / 180;
    m(1,1) = 1;
    m(2, 2) = cos(rad);
    m(2, 3) = sin(rad);
    m(3, 2) = -sin(rad);
    m(3, 3) = cos(rad);
    return m;
}

Matrix Transformations::rotateYMatrix(double angle) {
    Matrix m;
    double rad = angle * M_PI / 180;
    m(1, 1) = cos(rad);
    m(1, 3) = -sin(rad);
    m(2, 2) = 1;
    m(3, 1) = sin(rad);
    m(3, 3) = cos(rad);
    return m;
}

Matrix Transformations::rotateZMatrix(double angle) {
    Matrix m;
    double rad = angle * M_PI / 180;
    m(1, 1) = cos(rad);
    m(1, 2) = sin(rad);
    m(2, 1) = -sin(rad);
    m(2, 2) = cos(rad);
    m(3,3) = 1;
    m(4,4) = 1;
    return m;
}

Matrix Transformations::translateMatrix(const Vector3D &translation) {
    Matrix m;
    m(4, 1) = translation.x;
    m(4, 2) = translation.y;
    m(4, 3) = translation.z;
    return m;
}

Matrix Transformations::eyePointTrans(const Vector3D &eye) {
    double r = sqrt(eye.x * eye.x + eye.y * eye.y + eye.z * eye.z);
    double theta = atan2(eye.y, eye.x);
    double phi = acos(eye.z / r);
    Matrix m;

    // Row 1 (right vector)
    m(1,1) = -sin(theta);
    m(1,2) = -cos(theta) * cos(phi);
    m(1,3) =  cos(theta) * sin(phi);
    m(1,4) = 0;

    // Row 2 (up vector)
    m(2,1) =  cos(theta);
    m(2,2) = -sin(theta) * cos(phi);
    m(2,3) =  sin(theta) * sin(phi);
    m(2,4) = 0;

    // Row 3 (forward vector) -- flip its sign so the camera looks from the front
    m(3,1) = 0;
    m(3,2) = sin(phi);
    m(3,3) = cos(phi);
    m(3,4) = 0;

    // Translation row: also flip the sign on the z component
    m(4,1) = 0;
    m(4,2) = 0;
    m(4,3) = -r;
    m(4,4) = 1;

    return m;
}

Vector3D Transformations::doProjection(const Vector3D &point, double d) {
    return Vector3D::point((d * point.x) / point.z, (d * point.y) / point.z, point.z);
}