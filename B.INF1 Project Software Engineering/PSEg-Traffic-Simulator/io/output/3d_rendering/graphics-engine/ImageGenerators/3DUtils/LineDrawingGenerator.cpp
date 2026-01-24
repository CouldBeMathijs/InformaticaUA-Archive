//
// Created by adria on 3-4-2025.
//

#include "LineDrawingGenerator.h"

Figure3D LineDrawingGenerator::createLineDrawing(const ini::Configuration &config, const std::string &figureKey) {
    Figure3D figure;

    int nrPoints = config[figureKey]["nrPoints"].as_int_or_die();
    figure.points.reserve(nrPoints);

    for (int j = 0; j < nrPoints; j++) {
        std::string pointKey = "point" + std::to_string(j);
        auto pointValues = config[figureKey][pointKey].as_double_tuple_or_die();
        figure.points.push_back(Vector3D::point(
                pointValues[0],
                pointValues[1],
                pointValues[2]
        ));
    }

    int nrLines = config[figureKey]["nrLines"].as_int_or_die();

    figure.lines.reserve(nrLines);
    for (int j = 0; j < nrLines; j++) {
        std::string lineKey = "line" + std::to_string(j);
        auto lineIdx = config[figureKey][lineKey].as_int_tuple_or_die();
        figure.lines.emplace_back(lineIdx[0], lineIdx[1]);
    }

    return figure;
}