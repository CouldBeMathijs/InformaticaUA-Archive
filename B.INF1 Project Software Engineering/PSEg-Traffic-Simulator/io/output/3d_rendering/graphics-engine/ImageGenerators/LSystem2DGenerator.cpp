//
// Created by adria on 12-3-2025.
//

#include "LSystem2DGenerator.h"

img::EasyImage LSystem2DGenerator::generate(const ini::Configuration& config) {
    std::string inputFile = config["2DLSystem"]["inputfile"].as_string_or_die();
    ini::DoubleTuple lineColor = config["2DLSystem"]["color"].as_double_tuple_or_die();

    LParser::LSystem2D lSystem;
    std::ifstream file(inputFile);
    if (!file) {
        std::cerr << "Kan het L-System bestand niet openen: " << inputFile << std::endl;
        return img::EasyImage();
    }
    file >> lSystem;
    file.close();

    std::string systemString = generateLSystemString(lSystem);
    std::vector<std::pair<double, double>> points = processLSystemString(lSystem, systemString);

    return drawLSystemImage(config, points, lineColor);
}

std::string LSystem2DGenerator::generateLSystemString(const LParser::LSystem2D& lSystem) {
    std::string systemString = lSystem.get_initiator();
    for (int i = 0; i < lSystem.get_nr_iterations(); i++) {
        std::string newSystemString;
        for (char c : systemString) {
            if (lSystem.get_alphabet().count(c)) {
                newSystemString += lSystem.get_replacement(c);
            } else if (c == '+' || c == '-' || c == '(' || c == ')') {
                newSystemString += c;
            } else {
                std::cerr << "ERROR: Character '" << c << "' is not in the L-System alphabet!" << std::endl;
            }
        }
        systemString = newSystemString;
    }
    return systemString;
}

std::vector<std::pair<double, double>> LSystem2DGenerator::processLSystemString(const LParser::LSystem2D& lSystem, const std::string& systemString) {
    double angle = lSystem.get_starting_angle();
    double deltaAngle = lSystem.get_angle();
    std::vector<std::pair<double, double>> points;
    std::pair<double, double> position = {0.0, 0.0};
    std::pair<double, double> direction = {std::cos(angle * M_PI / 180), std::sin(angle * M_PI / 180)};
    std::stack<std::pair<std::pair<double, double>, std::pair<double, double>>> stateStack;

    points.push_back(position);
    for (char c : systemString) {
        if (c == '+') {
            double currentAngle = atan2(direction.second, direction.first) * 180 / M_PI;
            currentAngle += deltaAngle;
            direction = {cos(currentAngle * M_PI / 180), sin(currentAngle * M_PI / 180)};
        } else if (c == '-') {
            double currentAngle = atan2(direction.second, direction.first) * 180 / M_PI;
            currentAngle -= deltaAngle;
            direction = {cos(currentAngle * M_PI / 180), sin(currentAngle * M_PI / 180)};
        } else if (c == '(') {
            stateStack.emplace(position, direction);
        } else if (c == ')') {
            if (!stateStack.empty()) {
                position = stateStack.top().first;
                direction = stateStack.top().second;
                stateStack.pop();
                points.push_back(position);
            }
        } else if (lSystem.get_alphabet().count(c)) {
            if (lSystem.draw(c)) {
                position.first += direction.first;
                position.second += direction.second;
                points.push_back(position);
            }
        } else {
            std::cerr << "ERROR: Unexpected character '" << c << "' encountered!" << std::endl;
        }
    }
    return points;
}

img::EasyImage LSystem2DGenerator::drawLSystemImage(const ini::Configuration& config, const std::vector<std::pair<double, double>>& points, const ini::DoubleTuple& lineColor) {
    int size = config["General"]["size"].as_int_or_die();
    ini::DoubleTuple bgColor = config["General"]["backgroundcolor"].as_double_tuple_or_die();

    double minX = points[0].first, maxX = points[0].first;
    double minY = points[0].second, maxY = points[0].second;

    for (const auto& p : points) {
        minX = std::min(minX, p.first);
        maxX = std::max(maxX, p.first);
        minY = std::min(minY, p.second);
        maxY = std::max(maxY, p.second);
    }

    double width = maxX - minX;
    double height = maxY - minY;

    bool isWidthLargest = (width >= height);

    double scale;
    if (isWidthLargest) {
        scale = (0.95 * size) / width;
    } else {
        scale = (0.95 * size) / height;
    }

    double finalWidth, finalHeight;
    if (isWidthLargest) {
        finalWidth = (double)size;
        finalHeight = (height * scale) / 0.95;
    } else {
        finalHeight = (double)size;
        finalWidth = (width * scale) / 0.95;
    }

    int imageWidth  = (int)std::floor(finalWidth);
    int imageHeight = (int)std::floor(finalHeight);

    img::EasyImage image(imageWidth, imageHeight, img::Color(255 * bgColor[0], bgColor[1] * 255, bgColor[2] * 255));

    double offsetX = 0.025 * finalWidth  - (minX * scale);
    double offsetY = 0.025 * finalHeight - (minY * scale);

    img::Color color(lineColor[0] * 255, lineColor[1] * 255, lineColor[2] * 255);
    for (size_t i = 1; i < points.size(); i++) {
        int x0 = static_cast<int>(scale * points[i-1].first + offsetX);
        int y0 = static_cast<int>(scale * points[i-1].second + offsetY);
        int x1 = static_cast<int>(scale * points[i].first + offsetX);
        int y1 = static_cast<int>(scale * points[i].second + offsetY);

        // Clamping to prevent out-of-bounds errors
        x0 = std::max(0, std::min(x0, imageWidth - 1));
        y0 = std::max(0, std::min(y0, imageHeight - 1));
        x1 = std::max(0, std::min(x1, imageWidth - 1));
        y1 = std::max(0, std::min(y1, imageHeight - 1));

        image.draw_line(x0, y0, x1, y1, color);
    }

    return image;
}