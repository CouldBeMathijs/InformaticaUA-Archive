// Functions made for the session on 19/02/2025
#include "LSystems2D.h"
#include <fstream>
#include "General.h"
#include <stack>
#include <cmath>

std::vector<img::Line2D> l2DgenerateLines(const LParser::LSystem2D &lsystem, const img::Color &lineColor) {
    std::vector<img::Line2D> lines;

    // Step 1: Generate the final string by applying the replacement rules
    std::string currentString = lsystem.get_initiator();
    const std::set<char> &alphabet = lsystem.get_alphabet();
    for (unsigned int i = 0; i < lsystem.get_nr_iterations(); ++i) {
        std::string newString;
        for (char c: currentString) {
            if (alphabet.find(c) == alphabet.end()) {
                newString += c;
            } else {
                newString += lsystem.get_replacement(c);
            }
        }
        currentString = std::move(newString);
    }

    // Step 2: Interpret the final string to generate lines
    double x = 0.0, y = 0.0;
    double angle = lsystem.get_starting_angle();
    const double deltaAngle = lsystem.get_angle();
    std::stack<std::pair<double, double> > positionStack;
    std::stack<double> angleStack;

    for (char c: currentString) {
        if (c == '+') {
            angle += deltaAngle; // Turn right
        } else if (c == '-') {
            angle -= deltaAngle; // Turn left
        } else if (c == '(') {
            positionStack.emplace(x, y);
            angleStack.push(angle);
        } else if (c == ')') {
            if (!positionStack.empty() && !angleStack.empty()) {
                x = positionStack.top().first;
                y = positionStack.top().second;
                positionStack.pop();
                angle = angleStack.top();
                angleStack.pop();
            }
        } else if (lsystem.draw(c)) {
            double newX = x + std::cos(degToRad(angle));
            double newY = y + std::sin(degToRad(angle));

            img::Point2D startPoint(x, y);
            img::Point2D endPoint(newX, newY);
            lines.emplace_back(startPoint, endPoint, lineColor);

            x = newX;
            y = newY;
        } else {
            // Move forward without drawing
            x += std::cos(degToRad(angle));
            y += std::sin(degToRad(angle));
        }
    }

    return lines;
}

bool Lsystem2DLSystem(const ini::Configuration &conf, img::EasyImage &out) {
    // Check if the type is "2DLSystem"
    if (conf["General"]["type"].as_string_or_die() != "2DLSystem") {
        return false;
    }

    // Parse configuration
    int size = conf["General"]["size"].as_int_or_die();
    img::Color backgroundcolor = vecToColor(conf["General"]["backgroundcolor"].as_double_tuple_or_die());
    img::Color lineColor = vecToColor(conf["2DLSystem"]["color"].as_double_tuple_or_die());

    // Initialize the image with the background color
    out.clear(backgroundcolor);

    // Parse the L-System from the input file
    LParser::LSystem2D lsystem;
    std::string inputFile = conf["2DLSystem"]["inputfile"].as_string_or_die();
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + inputFile);
    }

    file >> lsystem;
    file.close();

    // Generate lines from the L-System
    std::vector<img::Line2D> lines = l2DgenerateLines(lsystem, lineColor);

    // Draw the lines onto the image
    out = img::EasyImage::draw2DLinesNoZBuf(lines, size, backgroundcolor);
    return true;
}
