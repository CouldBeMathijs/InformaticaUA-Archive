// Functions made for the session on 19/05/2024
#ifndef LSYSTEMS2D_H
#define LSYSTEMS2D_H
#include "external/easy_image.h"
#include "external/ini_configuration.h"
#include "external/l_parser.h"

std::vector<img::Line2D> l2DgenerateLines(const LParser::LSystem2D &lsystem, const img::Color &lineColor);

bool Lsystem2DLSystem(const ini::Configuration &conf, img::EasyImage &out);

#endif //LSYSTEMS2D_H
