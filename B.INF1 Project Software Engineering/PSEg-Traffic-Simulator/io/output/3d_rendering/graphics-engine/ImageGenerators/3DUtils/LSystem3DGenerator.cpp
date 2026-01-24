//
// Created by adria on 3-4-2025.
//

#include "LSystem3DGenerator.h"

#include <fstream>
#include <stack>

Figure3D LSystem3DGenerator::create3DLSystemDrawing(const ini::Configuration &config, const std::string &figureKey) {
    std::string inputFile = config[figureKey]["inputfile"].as_string_or_die();

    LParser::LSystem3D lSystem;
    std::ifstream file(inputFile);
    if (!file) {
        std::cerr << "Kan het L-System bestand niet openen: " << inputFile << std::endl;
        return Figure3D();
    }
    file >> lSystem;
    file.close();

    std::string systemString = lSystem.get_initiator();
    for (int i = 0; i < lSystem.get_nr_iterations(); i++) {
        std::string next;
        for (char c : systemString) {
            if (lSystem.get_alphabet().count(c)) {
                next += lSystem.get_replacement(c);
            } else {
                next += c;
            }
        }
        systemString = next;
    }

    std::vector<Vector3D> points;
    std::vector<std::pair<int, int>> lines;

    Vector3D position = Vector3D::point(0, 0, 0);
    Vector3D H = Vector3D::vector(1, 0, 0);
    Vector3D L = Vector3D::vector(0, 1, 0);
    Vector3D U = Vector3D::vector(0, 0, 1);

    std::stack<std::tuple<Vector3D, Vector3D, Vector3D, Vector3D>> stateStack;

    points.push_back(position);
    unsigned int lastIndex = 0;

    for (char c : systemString) {
        if (lSystem.get_alphabet().count(c) && lSystem.draw(c)) {
            Vector3D newPos = position + H;
            points.push_back(newPos);
            lines.emplace_back(lastIndex, points.size() - 1);
            lastIndex = points.size() - 1;
            position = newPos;
        }
        else if (c == '+') { // Rotate left around U
            double angle = lSystem.get_angle() * M_PI / 180;
            H = rotateAroundAxis(H, U, angle);
            L = rotateAroundAxis(L, U, angle);
        }
        else if (c == '-') { // Rotate right around U
            double angle = -lSystem.get_angle() * M_PI / 180;
            H = rotateAroundAxis(H, U, angle);
            L = rotateAroundAxis(L, U, angle);
        }
        else if (c == '^') { // Pitch up (rotate around L)
            double angle = -lSystem.get_angle() * M_PI / 180;
            H = rotateAroundAxis(H, L, angle);
            U = rotateAroundAxis(U, L, angle);
        }
        else if (c == '&') { // Pitch down (rotate around L)
            double angle = lSystem.get_angle() * M_PI / 180;
            H = rotateAroundAxis(H, L, angle);
            U = rotateAroundAxis(U, L, angle);
        }
        else if (c == '\\') { // Roll left (rotate around H)
            double angle = -lSystem.get_angle() * M_PI / 180;
            L = rotateAroundAxis(L, H, angle);
            U = rotateAroundAxis(U, H, angle);
        }
        else if (c == '/') { // Roll right (rotate around H)
            double angle = lSystem.get_angle() * M_PI / 180;
            L = rotateAroundAxis(L, H, angle);
            U = rotateAroundAxis(U, H, angle);
        }
        else if (c == '|') {
            H = -H;
            L = -L;
        }
        else if (c == '(') {
            stateStack.emplace(position, H, L, U);
        }
        else if (c == ')') {
            std::tie(position, H, L, U) = stateStack.top();
            stateStack.pop();
            points.push_back(position);
            lastIndex = points.size() - 1;
        }

        // Normalize after transformations
        H = Vector3D::normalise(H);
        L = Vector3D::normalise(L);
        U = Vector3D::normalise(U);
    }

    Figure3D fig;
    fig.points = std::move(points);
    fig.lines = std::move(lines);

    return fig;
}

Vector3D LSystem3DGenerator::rotateAroundAxis(const Vector3D &v, const Vector3D &k, double angle) {
    Vector3D k_normalized = Vector3D::normalise(k);
    return v * cos(angle)
         + Vector3D::cross(k_normalized, v) * sin(angle)
         + k_normalized * (Vector3D::dot(k_normalized, v)) * (1 - cos(angle));
}
