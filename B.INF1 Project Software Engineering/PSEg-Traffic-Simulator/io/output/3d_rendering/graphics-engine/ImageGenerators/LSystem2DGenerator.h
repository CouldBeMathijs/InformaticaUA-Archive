//
// Created by adria on 12-3-2025.
//

#ifndef LSYSTEM2DGENERATOR_H
#define LSYSTEM2DGENERATOR_H
#define _USE_MATH_DEFINES

#include "ImageGenerator.h"
#include "../external/l_parser.h"

#include <stack>
#include <vector>
#include <fstream>
#include <cmath>

class LSystem2DGenerator : public ImageGenerator{
public:
    img::EasyImage generate(const ini::Configuration& config) override;

private:
    static std::string generateLSystemString(const LParser::LSystem2D& lSystem);
    static std::vector<std::pair<double, double>> processLSystemString(const LParser::LSystem2D& lSystem, const std::string& systemString);

    static img::EasyImage drawLSystemImage(const ini::Configuration& config, const std::vector<std::pair<double, double>>& points, const ini::DoubleTuple& lineColor);
};



#endif //LSYSTEM2DGENERATOR_H
