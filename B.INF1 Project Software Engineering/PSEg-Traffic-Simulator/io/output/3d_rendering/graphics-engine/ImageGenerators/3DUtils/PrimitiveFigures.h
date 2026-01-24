//
// Created by adria on 3-4-2025.
//

#ifndef PRIMITIVEFIGURES_H
#define PRIMITIVEFIGURES_H

#include "../Figure3DGenerator.h"
#include "Utils.h"
#include "../../external/vector3d.h"

struct Figure3D;

class PrimitiveFigures {
public:
    static Figure3D createCube();
    static Figure3D createTetrahedron();
    static Figure3D createOctahedron();
    static Figure3D createIcosahedron();
    static Figure3D createDodecahedron();
    static Figure3D createCylinder(int n, double height);
    static Figure3D createCone(int n, double height);
    static Figure3D createSphere(int subdivisions);
    static Figure3D createTorus(double R, double r, int n, int m);

private:
    static void subdivideIco(std::vector<Vector3D> &points, std::vector<std::array<int, 3>> &triangles);
    static int getMiddlePoint(int i1, int i2, std::vector<Vector3D> &points, std::map<long long int, int> &middlePointCache);
};



#endif //PRIMITIVEFIGURES_H
