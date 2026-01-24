//
// Created by adria on 3-4-2025.
//

#ifndef LINEDRAWINGGENERATOR_H
#define LINEDRAWINGGENERATOR_H
#include "../Figure3DGenerator.h"


class LineDrawingGenerator {
public:
    static Figure3D createLineDrawing(const ini::Configuration &config, const std::string &figureKey);
};



#endif //LINEDRAWINGGENERATOR_H
