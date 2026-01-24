//
// Created by adria on 3-4-2025.
//

#ifndef LSYSTEM3DGENERATOR_H
#define LSYSTEM3DGENERATOR_H
#include "../Figure3DGenerator.h"
#include "../../external/vector3d.h"

struct Figure3D;

class LSystem3DGenerator {
public:
    static Figure3D create3DLSystemDrawing(const ini::Configuration &config, const std::string &figureKey);

private:
    static Vector3D rotateAroundAxis(const Vector3D &v, const Vector3D &k, double angle);
};



#endif //LSYSTEM3DGENERATOR_H
