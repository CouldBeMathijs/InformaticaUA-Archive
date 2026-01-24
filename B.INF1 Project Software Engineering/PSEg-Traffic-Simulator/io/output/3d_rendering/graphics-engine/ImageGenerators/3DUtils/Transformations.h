//
// Created by adria on 3-4-2025.
//

#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#define _USE_MATH_DEFINES

#include "../../external/vector3d.h"

#include <cmath>
#include <vector>
#include <array>

class Transformations {

public:
    static Matrix scaleMatrix(double scale);
    static Matrix rotateXMatrix(double angle);
    static Matrix rotateYMatrix(double angle);
    static Matrix rotateZMatrix(double angle);
    static Matrix translateMatrix(const Vector3D &translation);
    static Matrix eyePointTrans(const Vector3D &eye);
    static Vector3D doProjection(const Vector3D &point, double d);
};



#endif //TRANSFORMATIONS_H
