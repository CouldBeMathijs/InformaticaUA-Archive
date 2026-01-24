#ifndef GENERAL_H
#define GENERAL_H
#include "external/easy_image.h"
#include "external/vector3d.h"
#include <cstdint>
#include <iostream>
#include <vector>

uint8_t convertTo255(const double &color);

img::Color vecToColor(const std::vector<double> &vec);

double degToRad(const double &deg);

struct Face {
    /**
     *De indexen refereren naar punten in de ‘points’ vector van de Figure-klasse
     */
    std::vector<unsigned int> point_indexes;

    explicit Face(const std::vector<unsigned int> &point_indexes);

    Face() = default;

    [[nodiscard]] std::vector<Face> triangulate() const;
};

struct Figure {
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    img::Color ambientReflection;
    img::Color diffuseReflection;
    img::Color specularReflection;
    double reflectionCoefficient;

    void triangulate();

    void applyTransformation(const Matrix &m);
};

class Light
{
public:
    img::Color ambientLight;
    img::Color diffuseLight;
    img::Color specularLight;
};

#endif // GENERAL_H
